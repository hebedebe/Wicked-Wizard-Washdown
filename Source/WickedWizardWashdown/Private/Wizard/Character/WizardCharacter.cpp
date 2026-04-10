// ReSharper disable CppMemberFunctionMayBeConst

#include "WizardCharacter.h"

#include "Spells/BaseSpell.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spells/SpellEnums.h"
#include "Wizard/Controller/WizardController.h"
#include "Wizard/State/WizardState.h"

AWizardCharacter::AWizardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWizardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWizardCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(CastAction, ETriggerEvent::Started, this, &AWizardCharacter::CastHandler);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWizardCharacter::MoveHandler);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AWizardCharacter::JumpHandler);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWizardCharacter::StopJumpHandler);
		
		EnhancedInputComponent->BindAction(ComposeSpellLeftAction, ETriggerEvent::Started, this, &AWizardCharacter::ComposeSpellLeftHandler);
		EnhancedInputComponent->BindAction(ComposeSpellUpAction, ETriggerEvent::Started, this, &AWizardCharacter::ComposeSpellUpHandler);
		EnhancedInputComponent->BindAction(ComposeSpellRightAction, ETriggerEvent::Started, this, &AWizardCharacter::ComposeSpellRightHandler);
	}
}

AWizardController* AWizardCharacter::GetWizardController() const
{
	return Cast<AWizardController>(GetController());
}

AWizardState* AWizardCharacter::GetWizardState() const
{
	return GetPlayerState<AWizardState>();
}

void AWizardCharacter::CastCurrentSpell(const FVector2D Direction)
{
	const auto State = GetWizardState();
	const auto SpellClass = State->GetCurrentSpell();
	
	if (IsValid(SpellClass))
	{
		// If the spell is valid, cast it and then clear the buffer
		ABaseSpell* Spell = GetWorld()->SpawnActor<ABaseSpell>(SpellClass, GetActorTransform());
		Spell->Execute(
			{
				Direction,
				GetActorLocation(),
				this
			}
		);
		
		State->ClearSpellBuffer();
	} else
	{
		// If the spell is invalid, just clear the buffer so they can try again
		State->ClearSpellBuffer();
	}
}

void AWizardCharacter::CastHandler(const FInputActionValue& Value)
{
	const FVector2D Direction = Value.Get<FVector2D>();
	
	CastCurrentSpell(Direction);
}

void AWizardCharacter::MoveHandler(const FInputActionValue& Value)
{
	const FVector2D InputDirection = Value.Get<FVector2D>();
	const FVector MovementDirection = {
		0.0,
		InputDirection.X, // Players can only walk left and right on the screen
		0.0
	};
	AddMovementInput(MovementDirection);
}

void AWizardCharacter::JumpHandler(const FInputActionValue& Value)
{
	Jump();
}

void AWizardCharacter::StopJumpHandler(const FInputActionValue& Value)
{
	StopJumping();
}

void AWizardCharacter::ComposeSpellLeftHandler(const FInputActionValue& Value)
{
	GetWizardState()->BufferSpellComponent(ESpellComponent::Left);
}

void AWizardCharacter::ComposeSpellUpHandler(const FInputActionValue& Value)
{
	GetWizardState()->BufferSpellComponent(ESpellComponent::Up);
}

void AWizardCharacter::ComposeSpellRightHandler(const FInputActionValue& Value)
{
	GetWizardState()->BufferSpellComponent(ESpellComponent::Right);
}

void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();
}
