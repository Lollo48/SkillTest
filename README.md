Skill Test GP3 — AI Programmer Specialization
This project was developed as part of the AI Programmer Specialization, showcasing a professional, modular, and highly reactive approach to Unreal Engine 5 AI development.

DragonBoss AI is a cutting-edge artificial intelligence system, developed entirely in C++ for Unreal Engine 5, designed to control an epic dragon boss in a 3D environment.
It’s a massive expansion over Unreal’s base AI system, featuring dynamic flying, combo-based attacks, and intelligent investigation behaviors.

🚀 Main Features
🎯 Dragon States
Idle / Patrolling
▸ The dragon walks along a ground spline path.
▸ Upon reaching the end, it automatically takes off and continues patrolling in the air with a circular flight pattern.

Attack
▸ If an enemy enters its vision cone, the dragon approaches and launches a series of diverse combo attacks.
▸ Each attack has dedicated damage values, animations, and cooldowns, managed through an event-driven system.

Investigating
▸ If the target is lost, the dragon moves randomly within a configurable spherical search area.
▸ It remains in investigation mode for up to 100 seconds.
▸ If it spots the target again, it re-engages; otherwise, it returns to patrolling.

✈️ Dynamic Flying System
Automatic takeoff after completing ground patrols.

Aerial patrolling with smooth circular movement.

Flying behavior managed through the EFlyingActionState, with customizable flying times.

⚔️ Combo Attack System
Based on Data Assets (AttackDataAsset) for maximum flexibility.

Each combo defines:

Specific damage values,

Unique animations,

Individual cooldown timers.

Event-driven architecture (OnAttackStart) triggers visual and audio feedback dynamically.

🧠 AI Architecture
Controller: ABossController

Manages sight perception and target detection.

Handles dynamic state transitions (Passive, Patrolling, Investigating, Attacking, Flying).

Behavior Tree and Blackboard

Highly modular structure:

Custom Tasks, Services, and Decorators built in C++.

State synchronization using Blackboard keys (EnemyState, MovementActionState, etc.).

Stimuli Management

Advanced handling of lost sight events (OnLostSight) through timers for realistic behavior.
