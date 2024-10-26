set(TOP_LEVEL_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})

project("mswplugin_csharp" CSharp)

add_library("mswplugin_csharp" mswplugin_csharp.cs)

set_property(TARGET "mswplugin_csharp" PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.8")
set_property(TARGET "mswplugin_csharp" PROPERTY WIN32_EXECUTABLE FALSE)
set_property(TARGET "mswplugin_csharp" PROPERTY VS_CONFIGURATION_TYPE ClassLibrary)
set_property(TARGET "mswplugin_csharp" PROPERTY VS_DOTNET_REFERENCES
        "Microsoft.CSharp"
        "PresentationCore"
        "PresentationFramework"
        "System"
        "System.Core"
        "System.Data"
        "System.Data.DataSetExtensions"
        "System.Runtime.InteropServices"
        "System.Windows.Forms"
        "System.Net.Http"
        "System.Xaml"
        "System.Xml"
        "System.Xml.Linq"
        "WindowsBase")

set_target_properties("mswplugin_csharp" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TOP_LEVEL_PROJECT_SOURCE_DIR}/bin)
