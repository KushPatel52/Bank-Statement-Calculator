# app.py
from flask import Flask, render_template, request, redirect, url_for
from datetime import datetime, timedelta

app = Flask(__name__)

# Very basic in-memory database for demo purposes
accounts = {
    'user123': {
        'balance': 1000,
        'last_transaction': datetime.now(),
        'transactions': []
    }
}

interest_rate = 0.01  # Monthly interest rate

@app.route('/')
def index():
    return render_template('index.html', balance=accounts['user123']['balance'])

@app.route('/deposit', methods=['POST'])
def deposit():
    amount = float(request.form['amount'])
    accounts['user123']['balance'] += amount
    record_transaction('Deposit', amount)
    return redirect(url_for('index'))

@app.route('/withdraw', methods=['POST'])
def withdraw():
    amount = float(request.form['amount'])
    if amount > accounts['user123']['balance']:
        return "Insufficient funds"
    else:
        accounts['user123']['balance'] -= amount
        record_transaction('Withdrawal', -amount)
        return redirect(url_for('index'))

@app.route('/statement')
def statement():
    return render_template('statement.html', transactions=accounts['user123']['transactions'])

def record_transaction(description, amount):
    transaction = {
        'time': datetime.now(),
        'description': description,
        'amount': amount,
        'balance_after': accounts['user123']['balance']
    }
    accounts['user123']['transactions'].append(transaction)

if __name__ == '__main__':
    app.run(debug=True)

