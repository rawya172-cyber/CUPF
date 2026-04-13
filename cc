"""
===========================================================
Prime Classification System (Structural + Dynamic Model)
===========================================================

Author: [Rawia]
Date: [13.04.2026]

Intellectual Property Notice:
This work, including all algorithms, structural ideas,
mathematical formulations, and implementation, is the
original intellectual property of the author.

Unauthorized use, reproduction, modification, or
distribution of this code or its underlying concepts
without explicit permission is strictly prohibited.

This system combines:
- Weighted survival over prime divisors
- Dynamic elimination behavior
- Multi-scale analysis
- Fermat probabilistic validation

===========================================================
"""

import math
from sympy import primerange, isprime, primepi

# =========================================================
# Weighted Survival Function
# =========================================================
def weighted_survival(n, limit):
    score = 0
    total = 0
    
    for p in primerange(2, limit):
        w = 1 / p
        total += w
        
        if n % p != 0:
            score += w
    
    return score / total


# =========================================================
# Elimination Steps
# =========================================================
def elimination_steps(n):
    steps = 0
    for p in primerange(2, int(math.sqrt(n)) + 2):
        steps += 1
        if n % p == 0:
            return steps
    return steps


# =========================================================
# Square Detection
# =========================================================
def is_square(n):
    r = int(math.sqrt(n))
    return r * r == n


# =========================================================
# Fermat Test (multi-base)
# =========================================================
def fermat_test(n):
    bases = [2, 3, 5]
    for a in bases:
        if n <= a:
            continue
        if pow(a, n - 1, n) != 1:
            return False
    return True


# =========================================================
# Feature Extraction
# =========================================================
def extract_features(n):
    W1 = weighted_survival(n, 20)
    W2 = weighted_survival(n, 100)
    W3 = weighted_survival(n, 200)
    
    D1 = W2 - W1
    
    steps = elimination_steps(n)
    
    max_steps = primepi(int(math.sqrt(n)) + 1)
    ratio = steps / max_steps if max_steps > 0 else 0
    
    return W1, W2, W3, D1, steps, ratio


# =========================================================
# Main Classifier
# =========================================================
def classify(n):

    # Handle small numbers
    if n < 10:
        return "Prime" if n in [2, 3, 5, 7] else "Composite"

    # Remove perfect squares
    if is_square(n):
        return "Composite"
    
    W1, W2, W3, D1, steps, ratio = extract_features(n)
    
    # ------------------------
    # Structural classification
    # ------------------------
    
    if W3 > 0.97 and ratio > 0.9 and abs(D1) < 0.02:
        prediction = "Prime"
    
    elif W3 > 0.93 and ratio > 0.8:
        prediction = "Likely Prime"
    
    elif W3 < 0.85:
        prediction = "Composite"
    
    elif ratio < 0.6:
        prediction = "Composite"
    
    else:
        prediction = "Uncertain"
    
    # ------------------------
    # Fermat refinement
    # ------------------------
    
    if prediction in ["Prime", "Likely Prime", "Uncertain"]:
        if not fermat_test(n):
            return "Composite"
    
    return prediction


# =========================================================
# Testing Block
# =========================================================
if __name__ == "__main__":
    
    print(f"{'n':>3} {'Prediction':>18} {'Actual':>8}")
    print("-" * 35)
    
    for n in range(2, 100):
        print(f"{n:>3} {classify(n):>18} {str(isprime(n)):>8}")