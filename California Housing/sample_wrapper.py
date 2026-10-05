import subprocess
import json
import os
import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import numpy as np

# --- PROJECT ROOT ---
PROJECT_ROOT = os.path.dirname(os.path.abspath(__file__))

# --- CONFIGURATION ---
C_SOURCE = "main.c"
EXECUTABLE = "main"

DATA1_FILE = os.path.join(PROJECT_ROOT, "assets", "housing.csv")
DATA2_FILE = os.path.join(PROJECT_ROOT, "assets", "housing_clean.csv")
OUTPUT_IMAGE1 = os.path.join(PROJECT_ROOT, "assets", "price_income.png")
OUTPUT_IMAGE2 = os.path.join(PROJECT_ROOT, "assets", "price_income_clean.png")

# --- C EXECUTION HELPERS ---
def compile_c_code():
    subprocess.run(
        ["gcc", C_SOURCE, "-o", EXECUTABLE, "-lm"],
        cwd=os.path.join(PROJECT_ROOT,"c_modules"),
        check=True
    )

def run_c_analysis(clean=False):
    if not os.path.exists(os.path.join(PROJECT_ROOT,"c_modules",EXECUTABLE)):
        raise FileNotFoundError("Executable not found")

    args = [f"./{EXECUTABLE}", "--json"]
    if clean:
        args.append("--clean")

    result = subprocess.run(
        args,
        cwd=os.path.join(PROJECT_ROOT,"c_modules"),
        capture_output=True,
        text=True,
        check=True
    )

    return result.stdout

def parse_output(raw_output):
    for line in raw_output.strip().split('\n'):
        line = line.strip()
        if line.startswith("{"):
            return json.loads(line)
    return None

# --- VISUALIZATION LOGIC ---
def create_dashboard(data, clean=False):
    df = pd.read_csv(DATA2_FILE) if clean else pd.read_csv(DATA1_FILE)

    # Extract C-calculated stats
    slope = data['ax+b']['a']
    intercept = data['ax+b']['b']
    pearson_r = data['pearson']

    mean_price = data['price']['mn']
    sd_price = data['price']['sd']

    mean_income = data['income']['mn']
    sd_income = data['income']['sd']

    # Create a figure with a grid layout
    # Main scatter takes up the bottom-left, box plots on top and right
    fig = plt.figure(figsize=(14, 10))
    gs = gridspec.GridSpec(4, 4)

    # 1. Main Scatter Plot (Center)
    ax_scatter = fig.add_subplot(gs[1:4, 0:3])
    ax_scatter.scatter(df['median_income'], df['median_house_value'],
                       alpha=0.3, s=10, c='steelblue', label='Data Points')

    # Regression Line
    x_range = np.linspace(df['median_income'].min(), df['median_income'].max(), 100)
    y_pred = slope * x_range + intercept
    ax_scatter.plot(x_range, y_pred, color='crimson', linewidth=2,
                    label=f'Regression: y={slope:.0f}x+{intercept:.0f}')

    ax_scatter.set_xlabel('Median Income')
    ax_scatter.set_ylabel('Median House Value')
    ax_scatter.legend(loc='upper left')
    ax_scatter.grid(True, alpha=0.3)
    ax_scatter.text(0.05, 0.90, f"Pearson r: {pearson_r:.3f}", transform=ax_scatter.transAxes,
                    fontsize=12, bbox=dict(facecolor='white', alpha=0.8))

    # 2. Income Distribution (Top - Horizontal Box Plot)
    ax_hist_x = fig.add_subplot(gs[0, 0:3], sharex=ax_scatter)
    ax_hist_x.boxplot(df['median_income'], vert=False, widths=0.7, patch_artist=True,
                      boxprops=dict(facecolor='lightblue'))
    ax_hist_x.set_title("Distribution of Income (Top) & House Value (Right)")
    # Overlay Mean + SD from C
    ax_hist_x.axvline(mean_income, color='green', linestyle='--', label='Mean (C)')
    ax_hist_x.errorbar(mean_income, 1, xerr=sd_income, color='green', capsize=5, fmt='o', label='Mean ± SD')
    ax_hist_x.axis('off') # Hide axes for cleanliness

    # 3. Price Distribution (Right - Vertical Box Plot)
    ax_hist_y = fig.add_subplot(gs[1:4, 3], sharey=ax_scatter)
    ax_hist_y.boxplot(df['median_house_value'], vert=True, widths=0.7, patch_artist=True,
                      boxprops=dict(facecolor='lightcoral'))
    # Overlay Mean + SD from C
    ax_hist_y.axhline(mean_price, color='green', linestyle='--', label='Mean (C)')
    ax_hist_y.errorbar(1, mean_price, yerr=sd_price, color='green', capsize=5, fmt='o')
    ax_hist_y.axis('off')

    plt.tight_layout()
    OUTPUT_IMAGE = OUTPUT_IMAGE2 if clean else OUTPUT_IMAGE1
    plt.savefig(OUTPUT_IMAGE, dpi=300)
    print(f"📊 Dashboard saved to {OUTPUT_IMAGE}")

# --- MAIN ---
try:
    compile_c_code()
    output = run_c_analysis()
    data = parse_output(output)
    if data:
        create_dashboard(data)
    else:
        print("Error: No JSON data found.")
    output = run_c_analysis(True)
    data = parse_output(output)
    if data:
        create_dashboard(data, True)
    else:
        print("Error: No JSON data found.")
except Exception as e:
    print(f"An error occurred: {e}")
