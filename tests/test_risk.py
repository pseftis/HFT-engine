import pytest
from framework.client import EngineClient
import time

@pytest.fixture
def client():
    c = EngineClient()
    assert c.connect()
    yield c
    c.disconnect()

def test_max_position_rejection(client):
    # Max qty is 500. Order of 1000 should get rejected.
    client.send("PLACE_ORDER|201|TSLA|0|1|200.0|1000")
    time.sleep(0.1)
    msgs = client.read_all(timeout=0.2)
    # Status 1 = REJECTED
    assert len(msgs) > 0
    assert any("EVT:EXEC|201|1|0|0" in msg for msg in msgs)
    assert any("exceeds max position" in msg for msg in msgs)

def test_kill_switch(client):
    client.send("KILL_SWITCH")
    time.sleep(0.1)
    msgs = client.read_all(timeout=0.2)
    assert any("EVT:KILL_SWITCH_ACTIVE" in msg for msg in msgs)
    
    # Try placing an order after kill switch is active
    client.send("PLACE_ORDER|202|TSLA|0|1|200.0|10")
    time.sleep(0.1)
    msgs = client.read_all(timeout=0.2)
    assert any("EVT:EXEC|202|1|0|0" in msg for msg in msgs)
    assert any("Kill switch activated" in msg for msg in msgs)
