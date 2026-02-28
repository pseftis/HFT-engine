import pytest
import subprocess
import time
import os

if __name__ == "__main__":
    tests_dir = os.path.dirname(__file__)
    engine_exe = os.path.join(tests_dir, "..", "build", "Debug", "hft_engine.exe")
    
    if not os.path.exists(engine_exe):
        engine_exe = os.path.join(tests_dir, "..", "build", "hft_engine.exe")

    print(f"Starting HFT Engine: {engine_exe}")
    engine = subprocess.Popen([engine_exe], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    time.sleep(1) # wait for TCP server to bind
    
    print("\n==================================")
    print(" Running Full Regression Suite...")
    print("==================================\n")
    
    try:
        pytest.main(["-v", "-s", tests_dir])
    finally:
        print("\nShutting down HFT Engine...")
        engine.terminate()
        try:
            engine.wait(timeout=2)
        except subprocess.TimeoutExpired:
            engine.kill()
