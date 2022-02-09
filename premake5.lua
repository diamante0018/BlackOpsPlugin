dependencies = {
	basePath = "./deps"
}

function dependencies.load()
	dir = path.join(dependencies.basePath, "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function dependencies.imports()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.import()
		end
	end
end

function dependencies.projects()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.project()
		end
	end
end

dependencies.load()

workspace "black-ops-plugin"
location "./build"
objdir "%{wks.location}/obj"
targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"
targetname "%{prj.name}"

configurations {"Debug", "Release"}

language "C++"
cppdialect "C++20"

architecture "x86"
platforms "x86"

systemversion "latest"
symbols "On"
staticruntime "On"
editandcontinue "Off"
warnings "Extra"
characterset "ASCII"

flags { "NoIncrementalLink", "NoMinimalRebuild", "MultiProcessorCompile", "No64BitChecks" }

filter "platforms:x86"
	defines {"_WINDOWS", "WIN32"}
filter {}

filter "configurations:Release"
	optimize "Size"
	buildoptions {"/GL"}
	linkoptions { "/IGNORE:4702", "/LTCG" }
	defines {"NDEBUG"}
	flags {"FatalCompileWarnings"}
filter {}

filter "configurations:Debug"
	optimize "Debug"
	defines {"DEBUG", "_DEBUG"}
filter {}

project "black-ops-plugin"
kind "SharedLib"
language "C++"

files {"./src/**.hpp", "./src/**.cpp"}

includedirs {"src"}

dependencies.imports()

group "Dependencies"
dependencies.projects()
