# =========================
# Simple C++ Workspace Make
# (Makefile-only templates)
# =========================
# Purpose:
# - Scaffold small C++ apps from templates
# - Build/run apps individually or all at once
# - Keep a shared BUILD_DIR and clean artifacts safely

# Folder where apps live (each app is a subfolder)
APP_DIR      := apps
# Folder for build outputs (not deleted on clean)
BUILD_DIR    := build
# Folder containing scaffolding templates
TEMPLATE_DIR := templates/app

# Compiler (override via CXX=clang++)
CXX        ?= g++
# Debug|Release controls flags
BUILD_TYPE ?= Debug

# Common warnings + standard
CXXFLAGS_COMMON := -std=c++20 -Wall -Wextra -Wpedantic
# Linker flags (unused)
LDFLAGS_COMMON  :=

# Select compile flags based on BUILD_TYPE
ifeq ($(BUILD_TYPE),Debug)
	CXXFLAGS := $(CXXFLAGS_COMMON) -O0 -g     # No optimization + debug info
else ifeq ($(BUILD_TYPE),Release)
	CXXFLAGS := $(CXXFLAGS_COMMON) -O3        # High optimization
else
	$(error Invalid BUILD_TYPE '$(BUILD_TYPE)'. Use Debug or Release.)
endif

export CXX CXXFLAGS LDFLAGS_COMMON BUILD_DIR

# Helper: portable "mkdir -p" function
ifeq ($(OS),Windows_NT)
	MKDIR_P = powershell -NoProfile -ExecutionPolicy Bypass -Command "New-Item -ItemType Directory -Force -Path '$(1)' | Out-Null"
else
	MKDIR_P = mkdir -p "$(1)"
endif

# GNU Make helpers for simple templating
# - render: replace __APP_NAME__ with provided name
render = $(subst __APP_NAME__,$(2),$(1))
# - readfile: load a file into a variable
readfile = $(file <$(1))
# - writefile: write content into a file
writefile = $(file >$(1),$(2))

# Load templates into variables for scaffolding
TPL_MAIN_CPP     := $(call readfile,$(TEMPLATE_DIR)/main.cpp.tpl)
TPL_APP_MAKEFILE := $(call readfile,$(TEMPLATE_DIR)/Makefile.tpl)
TPL_README       := $(call readfile,$(TEMPLATE_DIR)/README.md.tpl)

# -------------------------
# App discovery
# -------------------------
# Discover apps by enumerating subfolders under APP_DIR
APPS := $(notdir $(wildcard $(APP_DIR)/*))

# -------------------------
# Targets
# -------------------------
.PHONY: help list app build run clean delete-app   # Declare non-file targets

help:   # Show available commands and variables
	@echo "Targets:"
	@echo "  make app app=my_app         Create new app scaffold (from templates/app/*.tpl)"
	@echo "  make list                   List apps"
	@echo "  make build [app=name]       Build one app (or all if omitted)"
	@echo "  make run  app=name          Run one app"
	@echo "  make clean                  Remove build outputs"
	@echo "  make delete-app app=name    Delete the specified app and its build artifacts"
	@echo ""
	@echo "Vars:"
	@echo "  BUILD_TYPE=Debug|Release"
	@echo "  CXX=g++|clang++|..."
	@echo "  app=name (for build/run)"

list:   # List discovered apps
	@printf "%s\n" $(APPS)

app:    # Scaffold a new app (requires: app=NAME); uses templates and sed substitution
ifndef app
	$(error Usage: make app app=<my_app>)
endif
	@$(call MKDIR_P,$(APP_DIR)/$(app)/src)
	@sed 's/__APP_NAME__/$(app)/g' "$(TEMPLATE_DIR)/main.cpp.tpl" > "$(APP_DIR)/$(app)/src/main.cpp"
	@sed 's/__APP_NAME__/$(app)/g' "$(TEMPLATE_DIR)/Makefile.tpl"  > "$(APP_DIR)/$(app)/Makefile"
	@sed 's/__APP_NAME__/$(app)/g' "$(TEMPLATE_DIR)/README.md.tpl" > "$(APP_DIR)/$(app)/README.md"
	@echo "Created app: $(APP_DIR)/$(app)"

build:  # Build one app (with app=NAME) or build all apps when app is omitted
ifdef app
	@$(MAKE) -C "$(APP_DIR)/$(app)" build
else
	@for a in $(APPS); do $(MAKE) -C "$(APP_DIR)/$$a" build; done
endif

run:    # Run a single app binary (requires: app=NAME)
ifndef app
	$(error Usage: make run app=name)
endif
	@$(MAKE) -C "$(APP_DIR)/$(app)" run

clean:  # Clean build artifacts under BUILD_DIR, keep BUILD_DIR itself
	@echo "Cleaning artifacts under $(BUILD_DIR) without deleting the folder"
	@if [ -d "$(BUILD_DIR)" ]; then rm -rf "$(BUILD_DIR)"/*; else echo "$(BUILD_DIR) does not exist; nothing to clean"; fi

delete-app:  # Delete an app folder and its build artifacts (requires: app=NAME)
ifndef app
	$(error Usage: make delete-app app=name)
endif
	@echo "Deleting app: $(app)"
	@if [ -d "$(APP_DIR)/$(app)" ]; then rm -rf "$(APP_DIR)/$(app)"; else echo "App '$(app)' not found in $(APP_DIR)"; fi
	@if [ -d "$(BUILD_DIR)" ]; then rm -rf "$(BUILD_DIR)/$(app)"; fi
	@echo "Delete complete for $(app)"
