#include "SetupDockspace.h"

void SetupDockspace()
{
    //Dockspace
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking; //ImGuiWindowFlags_MenuBar 
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiID DockspaceID = ImGui::GetID("DockSpace");

    if (!ImGui::DockBuilderGetNode(DockspaceID))
    {
        ImGui::DockBuilderRemoveNode(DockspaceID); // Clear out existing layout
        ImGui::DockBuilderAddNode(DockspaceID, dockspace_flags); // Add empty node
        ImGui::DockBuilderSetNodeSize(DockspaceID, ImGui::GetMainViewport()->Size);

        ImGuiID dock_main_id = DockspaceID; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        ImGuiID DockLeft = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.70f, nullptr, &dock_main_id);
        ImGuiID DockRight = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);
    	ImGuiID DockLeftChild = ImGui::DockBuilderSplitNode(DockLeft, ImGuiDir_Down, 0.90f, nullptr, &DockLeft);
    	ImGuiID DockRightChild = ImGui::DockBuilderSplitNode(DockRight, ImGuiDir_Down, 0.20f, nullptr, &DockRight);
    	ImGuiID DockingLeftDownChild = ImGui::DockBuilderSplitNode(DockLeftChild, ImGuiDir_Down, 0.06f, nullptr, &DockLeftChild);

        ImGui::DockBuilderDockWindow("Game Search", DockLeft);
        ImGui::DockBuilderDockWindow("Profiles", DockRight);
    	ImGui::DockBuilderDockWindow("GamesTable", DockLeftChild);
    	ImGui::DockBuilderDockWindow("TableButtons", DockingLeftDownChild);
    	ImGui::DockBuilderDockWindow("Logs", DockRightChild);

        // Disable tab bar for custom toolbar so that it remains docked as well as not allow other dockings (because we dont need to)
        ImGuiDockNode* node = ImGui::DockBuilderGetNode(DockLeft);
        node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
        node->LocalFlags |= ImGuiDockNodeFlags_NoDocking;
        node = ImGui::DockBuilderGetNode(DockRight);
        node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
        node->LocalFlags |= ImGuiDockNodeFlags_NoDocking;
    	node = ImGui::DockBuilderGetNode(DockLeftChild);
    	node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
        node->LocalFlags |= ImGuiDockNodeFlags_NoDocking;
    	node = ImGui::DockBuilderGetNode(DockingLeftDownChild);
    	node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
        node->LocalFlags |= ImGuiDockNodeFlags_NoDocking;
    	node = ImGui::DockBuilderGetNode(DockRightChild);
    	node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
        node->LocalFlags |= ImGuiDockNodeFlags_NoDocking;

        //Finish that shit
        ImGui::DockBuilderFinish(DockspaceID);
    }
    ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), dockspace_flags);
}

void EndDockspace()
{
    ImGui::End();
}
