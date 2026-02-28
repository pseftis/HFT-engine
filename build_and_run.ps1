Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
mkdir build
cd build
cmake ..
cmake --build . --config Debug
cd ..

Write-Host "Build complete. Setting up Python environment..."
python -m venv venv
.\venv\Scripts\Activate.ps1
pip install -r tests\requirements.txt

Write-Host "Running Tests..."
python tests\run_tests.py
