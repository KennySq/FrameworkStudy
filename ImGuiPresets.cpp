#include"stdafx.h"
#include"ImGuiPresets.h"

void ImGui_TransformEditor(Transform * const TRSComponent)
{
	ImGui::BeginChild("Transform");

	float Pos[] = { TRSComponent->GetPosition().m128_f32[0],
					TRSComponent->GetPosition().m128_f32[1],
					TRSComponent->GetPosition().m128_f32[2],
					TRSComponent->GetPosition().m128_f32[3]
				};

	ImGui::SliderFloat4("Position", Pos, -10.0f, 10.0f, "%.2f");

	TRSComponent->SetPosition(Pos[0], Pos[1], Pos[2]);

	ImGui::EndChild();
}

void ImGui_TransformViewer(Transform* const TRSComponent)
{
	ImGui::BeginChild("Trasnform");

	float Pos[] = { TRSComponent->GetPosition().m128_f32[0],
	TRSComponent->GetPosition().m128_f32[1], 
	TRSComponent->GetPosition().m128_f32[2], 
	TRSComponent->GetPosition().m128_f32[3]
	};

	ImGui::InputFloat4("Position", Pos, 2);

	ImGui::EndChild();
}

void ImGui_InstanceViewer(Instance* const Inst)
{
	ImGui::Begin((to_string(Inst->GetIID()) + " " + Inst->GetName()).c_str());

	ImGui_TransformViewer(Inst->GetComponent<Transform>());

	ImGui::End();
}

void ImGui_SceneViewer(Scene * const SceneInst)
{
	ImGui::Begin(SceneInst->GetSceneName().c_str());
	
	auto Insts = SceneInst->GetInstancs();
	Instance* SelectedInstance = nullptr;
	static bool Selected[2] = { false };

	//ImGui::Selectable(Insts[0]->GetName().c_str(), &Selected[0]);


	for (int i = 0; i < SceneInst->GetInstanceCount(); i++)
	{	
		if (ImGui::Selectable((Insts[i]->GetName() + to_string(i)).c_str(), &Selected[i]))
		{
			//Selected[i] = true;
		}
		
		
	}

	for (int i = 0; i < SceneInst->GetInstanceCount(); i++)
	{
		if (Selected[i])
		{
			SelectedInstance = Insts[i];

			ImGui::Text(("Selected Instance : " + SelectedInstance->GetName()).c_str());

		}
	}

	ImGui::End();

}
