def card_type(totalsum, number):

    length = len(number)

    if totalsum % 10 is not 0:
        return "INVALID"

    elif (length is 16 or length is 13) and number[0] is '4':
        return "VISA"

    elif length is 16 and number[0] is '5' and (number[1] > '0' and number[1] < '6'):
        return "MASTERCARD"

    elif length is 15 and number[0] is '3' and (number[1] > '4' and number[1] is '7'):
        return "AMEX"

    else:
        return "INVALID"

def lugns_algo(number):
    length = len(number)

    sum1_list = [int(number[i]) * 2 for i in range(len(number) - 2, -1 , -2)]

    sum1 = 0
    for i in sum1_list:
        for x in list(str(i)):
            sum1 += int(x)

    sum2 = sum(int(number[i]) for i in range(len(number) - 1, -1, -2 ))


    return sum1 + sum2



while True:
    number = input("Number:")

    if number.isdigit():
        break

totalsum = lugns_algo(number)
print(card_type(totalsum, number))

