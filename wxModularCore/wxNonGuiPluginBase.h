#pragma once

#include "Declarations.h"
#include "wxPluginBase.h"

class DEMO_API wxNonGuiPluginBase : public wxPluginBase
{
	DECLARE_ABSTRACT_CLASS(wxNonGuiPluginBase)
public:
	static constexpr bool is_gui_plugin = false;
	wxNonGuiPluginBase();
	virtual ~wxNonGuiPluginBase();
	
	virtual int Work() = 0;
};
