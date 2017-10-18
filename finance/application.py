from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/", methods=["GET", "POST"])
@login_required
def index():

    if request.method == "GET":
        #For current user
        #Which stocks a user owns
        # Get all transcation of a current stock - compute amount - if not 0 - find current value compute

        #Get cash balance
        current_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["cash"]

        #Query for users transactions
        everytransaction = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id = session["user_id"])

        #List to hold the holding of the user
        holdings = get_holdings(everytransaction)

        grand_total = 0;
        for i in list(holdings):
            grand_total += holdings[i].tot_val

        grand_total += current_cash

        return render_template("index.html", curr_cash = usd(current_cash), holdings = holdings, grand_total = usd(grand_total))

    if request.method == "POST":

        changes_stocks = {}

        #Query for users transactions
        everytransaction = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id = session["user_id"])

        #Get current holdins and get request changes
        holdings = get_holdings(everytransaction)

        print(holdings)

        for i in holdings.keys():
            if not request.form.get(i).lstrip('-+').isdigit():
                pass
            else:
                changes_stocks[i] = int(request.form.get(i))

        print(changes_stocks)

        for i in changes_stocks.keys():
            if changes_stocks[i] > 0:
                x = perform_buy(i, changes_stocks[i],db)
                if x != None:
                    return x
            else:
                print("SELLING!")
                x = perform_sell(i, abs(changes_stocks[i]),db)
                if x != None:
                    return x

        return redirect(url_for("index"))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    #Render form to buy shares
    if request.method == "GET":
        return render_template("buy.html")

    #User attemping transaction
    if request.method == "POST":
        quote_id = request.form.get("quote_id"); #Get quote
        number_of_shares = request.form.get("number_of_shares") #Get number of shares

        x = perform_buy(quote_id, number_of_shares, db)
        if x != None:
            return x

        return redirect(url_for("index"))

@app.route("/history")
@login_required
def history():

    #Query for users transactions
    everytransaction = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id = session["user_id"])

    #List to hold the holding of the user
    transactions = []

    #Iterate transactions
    for trans in everytransaction:
        if trans["quantity"] > 0:
            trans["type"] = "Purchase"
        else:
            trans["type"] = "Sale"

        trans["quantity"] = abs(trans["quantity"])
        transactions.append(trans)


    return render_template("history.html", transactions = transactions)



    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
        quote_name = request.form.get("quote_id")
        print(quote_name)
        quote_dict = lookup(quote_name)

        if not quote_dict:
            return apology("Stock Does Not Exist!")
        else:
            return render_template("quoted.html", name = quote_dict["name"], price = quote_dict["price"], symbol = quote_dict["symbol"])



@app.route("/register", methods=["GET", "POST"])
def register():

    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        if not request.form.get("password") and not request.form.get("username"):
            return apology("Please enter something! Do you want to register?")
        if not request.form.get("password"):
            return apology("Please enter a password to register")
        if not request.form.get("username"):
            return apology("Please enter a username to register")
        if request.form.get("password") != request.form.get("password2"):
            return apology("Passwords must match!")

        hash_val = pwd_context.hash(request.form.get("password")) #Get password hashvalue

        db.execute("INSERT INTO users (username, hash) VALUES (:username,:hash_val)", username = request.form.get("username"), hash_val = hash_val)
        return redirect(url_for("login"))


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    if request.method == "GET":
        return render_template("sell.html")

    if request.method == "POST":

        quote = request.form.get("quote_id")
        num_to_sell = request.form.get("num_to_sell")

        x = perform_sell(quote, num_to_sell, db)
        if x != None:
            return x

        return redirect(url_for("index"))

@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "GET":
        return render_template("addcash.html")

    if request.form.get("cash_to_add") == "" or not request.form.get("cash_to_add").isdigit():
        return apology("Please input a valid numeric value!")

    #Get added amount
    cash_to_add = float(request.form.get("cash_to_add"))

    if cash_to_add <= 0:
        return apology("You can only add! Zero does nothing!")

    #Get current cash
    current_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["cash"]

    #Add new cash
    current_cash += cash_to_add

    #Input new cash into database
    db.execute("UPDATE users SET cash = :n_cash WHERE id = :user_id", n_cash = current_cash, user_id = session["user_id"])

    #Redirect to index page
    return redirect(url_for("index"))



@app.route("/change_pass", methods=["GET", "POST"])
@login_required
def change_pass():


    if request.method == "GET":
        return render_template("change_pass.html")

    if request.method == "POST":
        # ensure password was submitted
        if not request.form.get("old_pass"):
            return apology("must provide password")

        # query database for username
        old_hash = db.execute("SELECT hash FROM users WHERE id = :user_id", user_id = session["user_id"])

        print(old_hash)

        print(request.form.get("old_pass"))

        # Check if password is correct
        if not pwd_context.verify(request.form.get("old_pass"), old_hash[0]["hash"]):
            return apology("Invalid Old Password")

        #Check if doubly typed new passwords match:
        if request.form.get("new_pass1") != request.form.get("new_pass2"):
            return apology("New passwords do not match")

        #Hash New Password
        hash_val = pwd_context.hash(request.form.get("new_pass1")) #Get password hashvalue

        db.execute("UPDATE users SET hash = :hash_val WHERE id = :user_id", user_id = session["user_id"], hash_val = hash_val)
        return redirect(url_for("index"))