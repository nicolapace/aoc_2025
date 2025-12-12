#!/usr/bin/env python3
import pulp as pl
import re


def parse_objective(objective_str):
    return [int(x) for x in objective_str.strip('{}').split(',')]


def parse_element(element_str, n_dimensions):
    positions = [int(x) for x in element_str.strip('()').split(',')]
    vector = [0] * n_dimensions
    for pos in positions:
        vector[pos] = 1
    return vector


def parse_elements(line):
    pattern = r'\([\d,]+\)'
    matches = re.findall(pattern, line)
    obj_match = re.search(r'\{[\d,]+\}', line)
    objective = parse_objective(obj_match.group())
    n_dimensions = len(objective)
    elements = [parse_element(match, n_dimensions) for match in matches]
    return objective, elements


def solve_milp(objective, elements):
    """
    Solve the MILP problem:
        minimize: sum(x)
        subject to: Ax = b, x >= 0, x integer
    """
    n_elements = len(elements)
    n_dimensions = len(objective)
    
    # Create the problem
    prob = pl.LpProblem("MinimumElements", pl.LpMinimize)
    
    # Decision variables: x[i] = how many times we use element i
    x = [pl.LpVariable(f"x_{i}", lowBound=0, cat='Integer') for i in range(n_elements)]
    
    # Objective function: minimize sum of all x[i]
    prob += pl.lpSum(x), "Total_Elements_Used"
    
    # Constraints: for each dimension, sum of (element[i][dim] * x[i]) must equal objective[dim]
    for dim in range(n_dimensions):
        prob += (
            pl.lpSum([elements[i][dim] * x[i] for i in range(n_elements)]) == objective[dim],
            f"Constraint_Dimension_{dim}"
        )
    
    # Solve the problem
    prob.solve(pl.PULP_CBC_CMD(msg=0))  # msg=0 suppresses solver output
    
    # Check if solution was found
    if prob.status == pl.LpStatusOptimal:
        solution_values = [int(v.varValue) for v in x]
        min_sum = int(pl.value(prob.objective))
        return min_sum, solution_values
    else:
        return None, None


def main():
    with open('input.txt', 'r') as f:
        lines = f.readlines()
    
    total = 0
    
    for line in lines:
        line = line.strip()
        if not line:
            continue
        
        # Parse the line
        objective, elements = parse_elements(line)
        
        print(f"Objective: {objective}")
        print(f"Elements: {elements}")
        
        # Solve using MILP
        result, solution = solve_milp(objective, elements)
        
        if result is not None:
            print(f"Minimum elements needed: {result}")
            print(f"Solution: {solution}")
            total += result
        else:
            print("No solution found!")
        print()
    
    print(f"Total: {total}")


if __name__ == "__main__":
    main()
