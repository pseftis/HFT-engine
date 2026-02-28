import pytest
from framework.client import EngineClient
import time

@pytest.fixture
def client():
    c = EngineClient()
    assert c.connect()
    yield c
    c.disconnect()

def test_latency(client):
    client.send("SET_LATENCY|50")
    time.sleep(0.1)
    client.read_all()
    
    latencies = []
    # Use smaller batch for pure reliability in test
    count = 100
    for i in range(count):
        client.send(f"PLACE_ORDER|3{i:03}|AAPL|0|1|150.0|10")
        
    time.sleep(0.5)
    msgs = client.read_all(1.0)
    for msg in msgs:
        if msg.startswith("EVT:EXEC"):
            parts = msg.split('|')
            if len(parts) > 5:
                latencies.append(int(parts[5])) # latency in micros
            
    assert len(latencies) > 0
    avg_latency = sum(latencies)/len(latencies)
    print(f"\n[Perf] Average Engine Latency: {avg_latency} µs over {len(latencies)} samples")

def test_throughput(client):
    start = time.time()
    count = 5000
    for i in range(count):
        client.send(f"PLACE_ORDER|4{i:04}|GOOG|1|1|200.0|5")
    
    received = 0
    while received < count:
        msgs = client.read_all(timeout=1.0)
        received += len(msgs)
        if time.time() - start > 5.0:
            break
            
    end = time.time()
    throughput = count / (end - start)
    print(f"\n[Perf] Throughput: {throughput:.2f} orders/sec")
    assert received > 0
