 Skill Test GP3

AI Programmer Specialization
Using C++, create task, service, and decorator nodes for an AI in Unreal and 3D, taking into account the following states: idle, attack, and investigating.
DragonBoss AI is an advanced AI created in C++ for Unreal Engine 5, implementing a dynamic behavior system for a dragon boss character.
This project:

Greatly extends the basic system,
Introduces dynamic flying behavior,
Supports combo-based attacks,
Part of the investigation in flight,
Integrates clear visual feedback for every state.

Main Features: 

Dragon States
Idle/Patrolling:
The dragon walks along a ground spline.
Once it reaches the final point, it takes off and continues patrolling by flying in a circular path.

Attack:
If the dragon spots an enemy inside it's vision cone, walks towards the target,launches a series of diversified combo attacks.

Investigating:
If the target disappears:
The dragon moves randomly inside a configurable spherical area,
It remains in search mode for 10 seconds (100 seconds in my case),
Then either returns to patrol if the enemy is not found or re-engages if it spots the target again.

Flying Behavior
After completing the ground patrol, the dragon automatically takes off,
Aerial patrolling is performed either along a circular mode,
Flying behavior is managed via the EFlyingActionState, with customizable flying durations.

Combo Attack System
Attacks are managed through AttackDataAsset,
Each combo attack includes:
Damage values,
Associated animations,
Separate cooldowns,
Event-driven attack system using OnAttackStart to trigger visual/audio feedback.

AI Structure
Controller: ABossController
Handles sight perception and target detection,
Manages dynamic state transitions (Passive, Patrolling, Investigating, Attacking, Flying).
Behavior Tree and Blackboard:
Highly modular via custom Tasks, Services, and Decorators,
State information is synchronized using Blackboard Keys (EnemyState, MovementActionState, etc.).
Stimuli Handling:
Advanced management of lost sight events (OnLostSight) using timers.
