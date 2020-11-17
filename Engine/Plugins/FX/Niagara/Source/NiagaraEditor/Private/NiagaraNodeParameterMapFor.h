// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SGraphPin.h"
#include "NiagaraNodeParameterMapSet.h"
#include "NiagaraNodeParameterMapFor.generated.h"

/** A node that allows a user to set multiple values into a parameter map in a for loop. */
UCLASS()
class UNiagaraNodeParameterMapFor : public UNiagaraNodeParameterMapSet
{
public:
	GENERATED_BODY()

	UNiagaraNodeParameterMapFor();

	virtual void AllocateDefaultPins() override;

	virtual void Compile(class FHlslNiagaraTranslator* Translator, TArray<int32>& Outputs) override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
