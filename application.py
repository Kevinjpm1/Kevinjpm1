from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    newid = session["user_id"]
    symbols = db.execute("SELECT symbol, shares FROM stocks WHERE id=:newid", newid=newid)

    gtot = 0

    for r in symbols:
        symbol = r["symbol"]
        sha = r["shares"]
        st = lookup(symbol)
        price = st["price"]
        total = price * sha
        gtot += total
        db.execute("UPDATE stocks set price=:p, subtotal=:tot WHERE symbol=:sym and id=:id", p=price, tot=total, sym=symbol, id=newid)

    money = db.execute("SELECT cash FROM users WHERE id=:newid", newid=newid)[0]["cash"]

    gtot = gtot + money

    portf = db.execute("SELECT * FROM stocks where id=:newid", newid=newid)

    return render_template("index.html", rows=portf, money=money, total=gtot)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")
    """Buy shares of stock"""
    newid = session["user_id"]
    money = db.execute("SELECT cash FROM users WHERE id=:newid", newid=session["user_id"])

    money = float(money[0]["cash"])

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        shareprice = lookup(request.form.get("symbol"))
        stocks = db.execute("SELECT shares FROM stocks WHERE id=:newid AND symbol=:symbol", newid=newid, symbol=symbol)

        if not shares or not symbol:
            return apology("Must fill all the blanks!", 403)

        if not shareprice:
            return apology("Share not found", 400)

        shareprice = shareprice["price"]

        if shares.isdigit() == False:
            return apology("No ALV", 400)

        shares = int(shares)

        if money < (shareprice * shares):
            return apology("You can´t afford that", 400)

        db.execute("INSERT INTO transactions(id, type, symbol, shares, shareprice, subtotal) VALUES(:newid, 'Buy', :symbol, :shares, :shareprice, :subtotal)",
                   newid=newid, symbol=symbol, shares=shares, shareprice=shareprice, subtotal=0 - (shares * shareprice))

        db.execute("UPDATE users SET cash = :newcash WHERE id=:newid", newcash=money - (shares * shareprice), newid=newid)

        if stocks:
            oldstocks = int(stocks[0]["shares"])
            subtotal = db.execute("SELECT subtotal FROM stocks WHERE id=:newid AND symbol=:symbol", newid=newid, symbol=symbol)
            subtotal = subtotal[0]["subtotal"]

            db.execute("UPDATE stocks SET shares=:newshares , subtotal=:subtotal WHERE id=:newid AND symbol=:newsymbol",
                       newid=newid, newsymbol=symbol, newshares=(oldstocks + shares), subtotal=subtotal + (shareprice * (oldstocks + shares)))

        else:
            db.execute("INSERT INTO stocks(id, symbol, shares, price, subtotal) VALUES(:newid, :symbol, :shares, :price, :subtotal)",
                       newid=newid, symbol=symbol, shares=shares, price=shareprice, subtotal=shareprice * shares)

        flash("Bought")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    newid = session["user_id"]
    """Show history of transactions"""
    rows = db.execute("SELECT date, type, symbol, shares, subtotal FROM transactions WHERE id=:newid",
                      newid=newid)
    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Loged in!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/addfounds", methods=["GET", "POST"])
@login_required
def addfounds():
    """Add founds!"""
    money = db.execute("SELECT cash FROM users WHERE id=:newid",
                       newid=session["user_id"])
    money = float(money[0]["cash"])
    if request.method == "POST":
        if not float(request.form.get("founds")):
            return apology("Must fill all the blanks", 403)

        founds = float(request.form.get("founds"))

        db.execute("UPDATE users SET cash = :newcash", newcash=money + founds)
        return redirect("/")
    else:
        return render_template("addfounds.html")


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """Change password"""
    if request.method == "POST":
        pw = request.form.get("newpassword")
        confirm = request.form.get("newconfirmation")
        if not password or not confirm:
            return apology("must fill all the blanks!", 403)

        if pw != confirm:
            return apology("passwords doesn´t match", 400)

        db.execute("UPDATE users SET hash=:newpassword WHERE id=:newid",
                   newid=session["user_id"], newpassword=generate_password_hash(pw))

        session.clear()
        return redirect("login.html")

    else:
        return render_template("password.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    symbol = request.form.get("symbol")
    if request.method == "POST":
        if not symbol:
            return apology("Must provide symbol!", 400)

        quote = lookup(symbol)
        if not quote:
            return apology("Share not found", 400)

        return render_template("quoted.html", quote=quote)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    session.clear()
    if request.method == "POST":
        if not request.form.get("username") or not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must fill all the blanks!", 400)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords doesn´t match!", 400)

        if db.execute("SELECT username FROM users WHERE username = :username",
                      username=request.form.get("username")):
            return apology("user is already taken!", 400)

        novoid = db.execute("INSERT INTO users('username', 'hash') VALUES(:username, :password)",
                            username=request.form.get("username"),
                            password=generate_password_hash(request.form.get("password")))
        session["user_id"] = novoid
        flash("Registered")
        return redirect("/")

    else:
        return render_template("/register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    newid = session["user_id"]
    nowstocks = db.execute("SELECT symbol FROM stocks WHERE id=:newid", newid=newid)
    if request.method == "GET":
        return render_template("sell.html", nowstocks=nowstocks)
    """Sell shares of stock"""
    money = db.execute("SELECT cash FROM users WHERE id=:newid",
                       newid=newid)
    money = float(money[0]["cash"])

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        shareprice = lookup(request.form.get("symbol"))
        stocks = db.execute("SELECT shares FROM stocks WHERE id=:newid AND symbol=:symbol",
                            newid=newid,
                            symbol=symbol)
        subtotal = db.execute("SELECT subtotal FROM stocks WHERE id=:newid AND symbol=:symbol",
                              newid=newid, symbol=symbol)

        if request.form.get("symbol") == 'None' or not shares:
            return apology("Must fill all the blanks", 403)

        if not stocks or not subtotal:
            return apology("You don´t have this", 404)

        if shares.isdigit() == False:
            return apology("No ALV", 400)

        shares = int(shares)

        shareprice = shareprice["price"]
        subtotal = subtotal[0]["subtotal"]
        stocks = int(stocks[0]["shares"])

        if stocks < shares:
            return apology("You don´t have this")

        db.execute("INSERT INTO transactions(id, type, symbol, shares, shareprice, subtotal) VALUES(:newid, 'Sell', :symbol, :shares, :shareprice, :subtotal)",
                   newid=newid, symbol=symbol, shares=shares, shareprice=shareprice, subtotal=(float(shares) * shareprice))

        db.execute("UPDATE users SET cash=:newcash WHERE id=:newid",
                   newid=newid, newcash=money + (shares * shareprice))

        db.execute("UPDATE stocks SET shares=:newshares , subtotal=:subtotal WHERE id=:newid AND symbol=:symbol",
                   newid=newid, symbol=symbol, newshares=stocks - shares, subtotal=shareprice * (stocks - shares))

        newstocks = db.execute("SELECT shares FROM stocks WHERE id=:newid AND symbol=:symbol", newid=newid, symbol=symbol)

        newstocks = int(newstocks[0]["shares"])

        if newstocks <= 0:
            db.execute("DELETE FROM stocks WHERE id=:newid AND symbol=:symbol", newid=newid, symbol=symbol)

        flash("Sold")
        return redirect("/")
    else:
        return render_template("sell.html")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
