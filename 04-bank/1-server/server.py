from flask import Flask, session, redirect, url_for
from flask import render_template
from flask import request
from flask import g
from flask import send_from_directory
import sqlite3

app = Flask(__name__, static_folder='static')
app.secret_key = 'A0Zr98j/3yX R~XHH!jmN]LWX/,?RT'

DATABASE = ":memory:" #'database.sql'
USER = 'b516bdde133b8628e160cb4b55698156ab76b2edbf3c84ed1825375772b94606'  #maciek
PASSWORD = 'abe31fe1a2113e7e8bf174164515802806d388cf4f394cceace7341a182271ab'  #haslo


def get_db():
    db = getattr(g, '_database', None)
    if db is None:
        db = g._database = sqlite3.connect(DATABASE)
        setup_database(db)
    return db


@app.teardown_appcontext
def close_connection(exception):
    db = getattr(g, '_database', None)
    if db is not None:
        db.close()


def setup_database(database):
    cursor = database.cursor()
    cursor.execute('CREATE TABLE IF NOT EXIST users(user TEXT, password TEXT)')
    cursor.execute('CREATE TABLE IF NOT EXIST payments(user TEXT, password TEXT)')
    cursor.execute('INSERT INTO users(user, password) VALUES(?,?)', (USER, PASSWORD))
    database.commit()


def validate(user, password):
    cursor = get_db().cursor()
    print "user=" + user
    cursor.execute('SELECT password FROM users WHERE user=\'' + user + '\'')
    for row in cursor:
        print "pass=", row[0]
        return password == row[0]
    return False


def do_the_login():
    if validate(request.form['user'], request.form['password']):
        session['username'] = "b" + request.form['user'] + "9"
        return redirect(url_for('index'))
    return render_template('login.html', txt="Nieprawidlowy login lub haslo!")


@app.route('/<path:path>')
def static_file(path):
    return send_from_directory('static', path)


@app.route('/index')
def index():
    if 'username' in session:
        return render_template('index.html')
    return redirect(url_for('login'))


@app.route('/make_payment', methods=['GET', 'POST'])
def make_payment():
    if request.method == 'POST':
        return redirect(url_for('confirm_payment',
                                account=request.form['account'],
                                name=request.form['name'],
                                address=request.form['address'],
                                amount=request.form['amount']))
    elif 'username' in session:
        return render_template('make_payment.html')
    return redirect(url_for('login'))


@app.route('/confirm_payment', methods=['GET', 'POST'])
def confirm_payment():
    if request.method == 'POST':
        account = request.form['account']
        name = request.form['name']
        address = request.form['address']
        amount = request.form['amount']
        return render_template('confirm_payment.html',
                               account=account, name=name, address=address,
                               amount=amount, confirmed=True)
    elif 'username' in session:
        account = request.args.get('account', type=int)
        name = request.args.get('name')
        address = request.args.get('address')
        amount = request.args.get('amount', type=int)
        return render_template('confirm_payment.html',
                               account=account, name=name, address=address, amount=amount)
    return redirect(url_for('login'))


@app.route("/")
@app.route('/login', methods=['GET', 'POST'])
def login():
    if 'username' in session:
        return redirect(url_for('index'))
    if request.method == 'POST':
        return do_the_login()
    else:
        return render_template('login.html')


if __name__ == "__main__":
    app.run()
