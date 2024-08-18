#pragma once

#include <wx/object.h>      // wxObject

class wxPluginBase : public wxObject {
public:
	virtual ~wxPluginBase(void) = default;
};

typedef wxPluginBase *(*CreatePlugin_function)(void);
typedef void (*DeletePlugin_function)(wxPluginBase*);
