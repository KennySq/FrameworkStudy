#include"stdafx.h"
#include"ImGuiPresets.h"

void ImGui_TransformViewer(Transform* const TRSComponent)
{
	ImGui::BeginChild("Trasnform");

	float Pos[] = { TRSComponent->GetPosition().m128_f32[0],
	TRSComponent->GetPosition().m128_f32[1], 
	TRSComponent->GetPosition().m128_f32[2], 
	TRSComponent->GetPosition().m128_f32[3], };

	ImGui::InputFloat4("Position", Pos, 2);

	ImGui::EndChild();
}

void ImGui_InstanceViewer(Instance* const Inst)
{
	ImGui::Begin((to_string(Inst->GetIID()) + " " + *Inst->GetName()).c_str());

	ImGui_TransformViewer(Inst->GetComponent<Transform>());

	ImGui::End();
}
