#pragma once
#include <d3d11.h>

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <dxgi.h>

#include "imgui_internal.h"
namespace PieMenu
{
	/* Declaration */
	bool BeginPiePopup(const char* pName, int iMouseButton = 0);
	void EndPiePopup();

	bool PieMenuItem(const char* pName, bool bEnabled = true);
	bool BeginPieMenu(const char* pName, bool bEnabled = true);
	void EndPieMenu();
}



///* Example */
//std::string sText;
//
//if( ImGui::IsWindowHovered() && ImGui::IsMouseClicked( 1 ) )
//{
//  ImGui::OpenPopup( "PieMenu" );
//}
//
//if( BeginPiePopup( "PieMenu", 1 ) )
//{
//  if( PieMenuItem( "Test1" ) ) sText = "Test1";
//  if( PieMenuItem( "Test2" ) )
//  {
//    sText = "Test2";
//    new MyImwWindow3();
//  }
//  if( PieMenuItem( "Test3", false ) ) sText = "Test3";
//  if( BeginPieMenu( "Sub" ) )
//  {
//    if( BeginPieMenu( "Sub sub\nmenu" ) )
//    {
//      if( PieMenuItem( "SubSub" ) ) sText = "SubSub";
//      if( PieMenuItem( "SubSub2" ) ) sText = "SubSub2";
//      EndPieMenu();
//    }
//    if( PieMenuItem( "TestSub" ) ) sText = "TestSub";
//    if( PieMenuItem( "TestSub2" ) ) sText = "TestSub2";
//    EndPieMenu();
//  }
//  if( BeginPieMenu( "Sub2" ) )
//  {
//    if( PieMenuItem( "TestSub" ) ) sText = "TestSub";
//    if( BeginPieMenu( "Sub sub\nmenu" ) )
//    {
//      if( PieMenuItem( "SubSub" ) ) sText = "SubSub";
//      if( PieMenuItem( "SubSub2" ) ) sText = "SubSub2";
//      EndPieMenu();
//    }
//    if( PieMenuItem( "TestSub2" ) ) sText = "TestSub2";
//    EndPieMenu();
//  }
//
//  EndPiePopup();
//}
