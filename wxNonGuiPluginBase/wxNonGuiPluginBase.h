#pragma once

#include "Declarations.h"

class DEMO_API wxNonGuiPluginBase : public wxObject
{
	DECLARE_ABSTRACT_CLASS(wxNonGuiPluginBase)
public:
	static constexpr bool is_plugin     = true ;
	static constexpr bool is_gui_plugin = false;
	wxNonGuiPluginBase();
	virtual ~wxNonGuiPluginBase();
	
	virtual int Work() = 0;
};

typedef wxNonGuiPluginBase * (*CreatePlugin_function)();
typedef void (*DeletePlugin_function)(wxNonGuiPluginBase * plugin);