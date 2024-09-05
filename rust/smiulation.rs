fn main() {
    let strategies = vec![
        CompositeStrategy::new(&[StrategyComponent::Random]),
        CompositeStrategy::new(&[StrategyComponent::HighestValue]),
        CompositeStrategy::new(&[StrategyComponent::HighestProbability]),
        CompositeStrategy::new(&[StrategyComponent::BalancedValue]),
        CompositeStrategy::new(&[StrategyComponent::Adaptive, StrategyComponent::ScoreManagement]),
        CompositeStrategy::new(&[StrategyComponent::LookAhead, StrategyComponent::ScoreManagement]),
    ];

    let results = run_tournament(&strategies, 10000);

    // Print or analyze results
    for (i, wins, total_games) in results {
        println!("Strategy {}: Win rate = {:.2}%", i, (wins as f64 / total_games as f64) * 100.0);
    }
}