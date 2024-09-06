import click
import json
from game_logic import run_simulation, Strategy

@click.command()
@click.option('--mode', type=click.Choice(['base', 'extended']), default='base', help='Game mode')
@click.option('--optimization', type=click.Choice(['standard', 'fast', 'ultra']), default='standard', help='Optimization level')
@click.option('--strategies', '-s', multiple=True, help='Strategies to use in the simulation')
@click.option('--num-games', default=10000, help='Number of games to simulate')
@click.option('--output', default='simulation_results.json', help='Output file for results')
def simulate(mode, optimization, strategies, num_games, output):
    """Run a dice game simulation with the specified parameters."""
    if not strategies:
        strategies = ['Random', 'HighestValue', 'HighestProbability', 'BalancedValue', 'Adaptive', 'LookAhead']
    
    strategy_objects = [Strategy.from_name(s) for s in strategies]
    results = run_simulation(mode, optimization, strategy_objects, num_games)
    
    with open(output, 'w') as f:
        json.dump(results, f, indent=2)
    
    click.echo(f"Simulation complete. Results saved to {output}")

if __name__ == '__main__':
    simulate()