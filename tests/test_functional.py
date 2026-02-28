import pytest
from framework.client import EngineClient
import time

@pytest.fixture
def client():
    c = EngineClient()
    assert c.connect()
    yield c
    c.disconnect()

def test_order_execution(client):
    # PLACE_ORDER|ID|SYMBOL|SIDE|TYPE|PRICE|QTY
    # ID=101, AAPL, BUY(0), MKT(1), 150.0, 10
    client.send("PLACE_ORDER|101|AAPL|0|1|150.0|10")
    time.sleep(0.1)
    msgs = client.read_all(timeout=0.2)
    assert len(msgs) > 0
    # Expected: EVT:EXEC|101|3|10|150|... 
    # Status 3 = FILLED
    assert any("EVT:EXEC|101|3|10|150" in msg for msg in msgs)

def test_partial_fill(client):
    # ID=102, MSFT, BUY(0), LIMIT(0), 100.0, 150 (qty > 100 -> partial fill)
    client.send("PLACE_ORDER|102|MSFT|0|0|100.0|150")
    time.sleep(0.1)
    msgs = client.read_all(timeout=0.2)
    assert len(msgs) > 0
    # Status 2 = PARTIALLY_FILLED
    assert any("EVT:EXEC|102|2|100|100" in msg for msg in msgs)

def test_cancel_order(client):
    client.send("PLACE_ORDER|103|MSFT|0|0|10.0|150")
    time.sleep(0.1)
    client.read_all(timeout=0.2)

    client.send("CANCEL_ORDER|103")
    time.sleep(0.1)
    msgs = client.read_all(timeout=0.2)
    # Status 4 = CANCELED
    assert any("EVT:EXEC|103|4|0|0" in msg for msg in msgs)
