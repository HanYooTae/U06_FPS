#include "CSessionRow.h"
#include "CMenu.h"
#include "Components/Button.h"

void UCSessionRow::SetSelfIndex(class UCMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	SelfIndex = InIndex;

	RowButton->OnClicked.AddDynamic(this, &UCSessionRow::OnClicked);
}

void UCSessionRow::OnClicked()
{
	Parent->SetSelectedRowIndex(SelfIndex);
}
