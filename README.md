<h1 align="center">
  Skill Test GP3
 <h4 align="center"> Lorenzo Grasso </h4>
</h1>

<h4 align="center">
AI Programmer Specialization </h4>
<h4 align="center">prototype of a dragon </h4>

Using C++, create task, service, and decorator nodes for an AI in Unreal and 3D, taking into account the following states: idle, attack, and investigating.
DragonBoss AI is an advanced AI created in C++ for Unreal Engine 5, implementing a dynamic behavior system for a dragon boss character.

This project:

Greatly extends the basic system,
Introduces dynamic flying behavior,
Supports combo-based attacks,
Part of the investigation in flight,
Integrates clear visual feedback for every state.

## Key Features

* Idle/Patrolling:
  - The dragon walks along a ground spline.
    Once it reaches the final point, it takes off and continues patrolling by flying in a circular path.

* Attack:
  - If the dragon spots an enemy inside it's vision cone, walks towards the target,launches a series of diversified combo attacks.
  
* Investigating:
  - If the target disappears:
    The dragon moves randomly inside a configurable spherical area,
    It remains in search mode for 10 seconds (100 seconds in my case),
    Then either returns to patrol if the enemy is not found or re-engages if it spots the target again.

* Flying Behavior
  - After completing the ground patrol, the dragon automatically takes off,
    Aerial patrolling is performed either along a circular mode,
    Flying behavior is managed via the EFlyingActionState, with customizable flying durations.

## More Information

## DragonBoss — Dynamic Combo Attack System

The DragonBoss features a dynamic, priority-based combo attack system that intelligently selects and manages attacks during combat, ensuring a fluid and responsive AI behavior.

* UAttackDataAsset — Attack Configuration

Each individual attack is defined inside a UAttackDataAsset.
This data-driven approach allows easy tuning of each attack’s properties without touching code.

- AttackType — Enum identifying the attack category.
- AttackDataID — Unique identifier for the attack.
- Damage — Amount of damage dealt.
- Cooldown — Time during which the attack cannot be reused.
- MinRange & MaxRange — Valid distance range for executing the attack.
- Priority — Higher-priority attacks are selected first when multiple are available.
- AttackMontage — Associated animation to play during the attack.
- bCanPerformAttack — Boolean flag controlling if the attack is currently available.



Attack Selection Logic — GetAttackDataAsset()
  
When it’s time for the dragon to perform an attack, the AI calls GetAttackDataAsset() to dynamically choose the most suitable attack:
Step-by-step:
Measure distance to the target.
Loop through available attacks (AttacksCombo array).

Filter out:

Attacks currently on cooldown (bCanPerformAttack == false).
Specific excluded attack types (e.g., ComboAttack1).

Select the attack:

Only attacks within the valid MinRange and MaxRange are considered.
The attack with the highest Priority wins.
Mark selected attack:
Set bCanPerformAttack = false.
Start a cooldown timer by adding it into AttackCooldownMap.
Remove attack from active combos temporarily.
If no valid attack is found, the system defaults to the first available attack.

Cooldown Management — UpdateCooldownAttacksCombo(float DeltaTime)
This method keeps track of attack cooldowns every tick:

Decreases the cooldown timer for each attack inside AttackCooldownMap.
When the cooldown reaches 0, the attack is:
Marked as available again (bCanPerformAttack = true).
Removed from AttackCooldownMap.
Re-added to the available AttacksCombo pool.
This ensures that attacks are automatically recycled and ready for use again after their cooldown expires.
Attack Selection Integration — BTTask_DragonSelectionComboAttack
The selection of the right attack is integrated into the Behavior Tree through the custom task UBTTask_DragonSelectionComboAttack:
It calls GetAttackDataAsset() on the dragon.
Stores the selected AttackType into the Blackboard (SelectedAttackKey).
This allows the Behavior Tree to trigger the corresponding attack animations or logic seamlessly.

In Short:

The DragonBoss:
Dynamically selects attacks based on distance, priority, and availability.
Manages cooldowns automatically and recycles attacks when ready.
Integrates with the Behavior Tree via a dedicated selection task for clean and modular AI behavior.

## AI Structure

Controller: ABossController

Handles sight perception and target detection,
Manages dynamic state transitions (Passive, Patrolling, Investigating, Attacking, Flying).

Behavior Tree and Blackboard:

Highly modular via custom Tasks, Services, and Decorators,
State information is synchronized using Blackboard Keys (EnemyState, MovementActionState, etc.).
Stimuli Handling:
Advanced management of lost sight events (OnLostSight) using timers.
