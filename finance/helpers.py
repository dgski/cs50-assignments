import csv
import urllib.request

from flask import redirect, render_template, request, session, url_for
from functools import wraps

def apology(top="", bottom=""):
    """Renders message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
            ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=escape(top), bottom=escape(bottom))

def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.11/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect(url_for("login", next=request.url))
        return f(*args, **kwargs)
    return decorated_function

def lookup(symbol):
    """Look up quote for symbol."""

    # reject symbol if it starts with caret
    if symbol.startswith("^"):
        return None

    # reject symbol if it contains comma
    if "," in symbol:
        return None

    # query Yahoo for quote
    # http://stackoverflow.com/a/21351911
    try:
        url = "http://download.finance.yahoo.com/d/quotes.csv?f=snl1&s={}".format(symbol)
        webpage = urllib.request.urlopen(url)
        datareader = csv.reader(webpage.read().decode("utf-8").splitlines())
        row = next(datareader)
    except:
        return None

    # ensure stock exists
    try:
        price = float(row[2])
    except:
        return None

    # return stock's name (as a str), price (as a float), and (uppercased) symbol (as a str)
    return {
        "name": row[1],
        "price": price,
        "symbol": row[0].upper()
    }

def usd(value):
    """Formats value as USD."""
    return "${:,.2f}".format(value)


class Holding:

    def __init__(self,i_quote, i_num_of_shares, i_curr_price):
        self.quote = i_quote
        self.num_of_shares = i_num_of_shares
        self.curr_price = i_curr_price
        self.tot_val = self.num_of_shares * self.curr_price

    def add_shares(self,quantity):
        self.num_of_shares += quantity
        self.tot_val = self.num_of_shares * self.curr_price

    def str_price(self):
        return usd(self.curr_price)

    def str_tot(self):
        return usd(self.tot_val)


def get_holdings(everytransaction):

    #List to hold the holding of the user
    holdings = {}

    #Iterate transactions
    for trans in everytransaction:

        quote = trans["stock"]
        num_shares = trans["quantity"]

        if quote in holdings:
            holdings[quote].add_shares(num_shares)
        else:
            current_price = lookup(quote)["price"]
            holdings[quote] = Holding(quote, num_shares, current_price)


    for i in list(holdings):
        if holdings[i].num_of_shares == 0:
            holdings.pop(i, None)

    return holdings

def perform_buy(quote_id, number_of_shares, db):

    quote_dict = lookup(quote_id) #Look up stock info using quote

    #If quote empty: issue apology
    if quote_id == "" or not quote_dict:
        return apology("Please enter something valid!")

    #If number of shares not greater than 0
    if int(number_of_shares) <= 0:
        return apology("At least try to buy one share!")

    print("LOOOOOOL")

    #Calculate transaction cost and check if transaction is feasible
    transaction_cost = quote_dict["price"] * float(number_of_shares)
    print(transaction_cost)
    print(session["user_id"])
    current_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["cash"]
    print(current_cash)
    if transaction_cost > current_cash:
        return apology("You cannot afford this!")

    #New cash amount to put into database
    new_cash = current_cash - transaction_cost

    #Place transaction into database:
    db.execute("INSERT into transactions (user_id, stock, quantity, price) VALUES (:user_id, :stock, :quantity, :price)",
    user_id = session["user_id"], stock = quote_id, quantity = int(number_of_shares), price = quote_dict["price"])

    #update user cash amount
    db.execute("UPDATE users SET cash = :n_cash WHERE id = :user_id", n_cash = new_cash, user_id = session["user_id"])

def perform_sell(quote, num_to_sell, db):
    if not quote or not num_to_sell:
        return apology("Please enter something proper in both fields!")

    num_to_sell = int(num_to_sell)

    if not num_to_sell > 0:
        return apology("At least sell one stock!")

    current_price = lookup(quote)["price"]

    #Get current quantity
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = :user_id AND stock = :quote", user_id = session["user_id"], quote = quote)

    current_number_of_shares = 0

    for trans in transactions:
        current_number_of_shares += trans["quantity"];

    if current_number_of_shares < num_to_sell:
        return apology("You do not own enough to sell this many!")

    #Perform actual sale

    #Get current cash
    current_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["cash"]
    current_cash += current_price * num_to_sell #add cash from sale

    #Put sale into database
    db.execute("INSERT into transactions (user_id, stock, quantity, price) VALUES (:user_id, :stock, :quantity, :price)",
    user_id = session["user_id"], stock = quote, quantity = -num_to_sell, price = current_price)

    #Update current cash

    db.execute("UPDATE users SET cash = :n_cash WHERE id = :user_id", n_cash = current_cash, user_id = session["user_id"])
