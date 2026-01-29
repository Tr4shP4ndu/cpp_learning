# =========================
# Simple C++ Workspace Make
# =========================
# Purpose:
# - Scaffold small C++ apps from templates
# - Build/run apps individually or all at once
# - Keep a shared BUILD_DIR and clean artifacts safely

# -------------------------
# Configuration
# -------------------------
APP_DIR      := apps
BUILD_DIR    := build
TEMPLATE_DIR := templates/app

CXX        ?= g++
BUILD_TYPE ?= Debug

CXXFLAGS_COMMON := -std=c++20 -Wall -Wextra -Wpedantic

# Build type flags
CXXFLAGS_Debug   := $(CXXFLAGS_COMMON) -O0 -g
CXXFLAGS_Release := $(CXXFLAGS_COMMON) -O3
CXXFLAGS         := $(CXXFLAGS_$(BUILD_TYPE))
$(if $(CXXFLAGS),,$(error Invalid BUILD_TYPE '$(BUILD_TYPE)'. Use Debug or Release.))

export CXX CXXFLAGS BUILD_DIR

# -------------------------
# Helpers
# -------------------------
MKDIR_P = mkdir -p "$(1)"
ifeq ($(OS),Windows_NT)
  MKDIR_P = powershell -NoProfile -ExecutionPolicy Bypass -Command "New-Item -ItemType Directory -Force -Path '$(1)' | Out-Null"
endif

# Template files: auto-discover all .tpl files
TEMPLATES := $(wildcard $(TEMPLATE_DIR)/*.tpl)

# -------------------------
# App discovery
# -------------------------
APPS := $(notdir $(wildcard $(APP_DIR)/*))

# -------------------------
# Require app= macro
# -------------------------
define require_app
$(if $(app),,$(error Usage: make $(1) app=<name>))
endef

# -------------------------
# Targets
# -------------------------
.PHONY: help list app build run clean delete-app

help:
	@echo "Targets:"
	@echo "  make app app=<name>         Create new app scaffold"
	@echo "  make list                   List apps"
	@echo "  make build [app=<name>]     Build one app (or all if omitted)"
	@echo "  make run app=<name>         Run one app"
	@echo "  make clean                  Remove build outputs"
	@echo "  make delete-app app=<name>  Delete app and its build artifacts"
	@echo ""
	@echo "Variables:"
	@echo "  BUILD_TYPE=Debug|Release (default: Debug)"
	@echo "  CXX=g++|clang++|... (default: g++)"

list:
	@printf "%s\n" $(APPS)

app:
	$(call require_app,$@)
	@$(call MKDIR_P,$(APP_DIR)/$(app)/src)
	@for tpl in $(TEMPLATES); do \
		outfile=$$(basename "$$tpl" .tpl); \
		case "$$outfile" in \
			main.cpp) dest="$(APP_DIR)/$(app)/src/main.cpp" ;; \
			*)        dest="$(APP_DIR)/$(app)/$$outfile" ;; \
		esac; \
		sed 's/__APP_NAME__/$(app)/g' "$$tpl" > "$$dest"; \
	done
	@echo "Created app: $(APP_DIR)/$(app)"

build:
ifdef app
	@$(MAKE) -C "$(APP_DIR)/$(app)" build
else
	@for a in $(APPS); do $(MAKE) -C "$(APP_DIR)/$$a" build; done
endif

run:
	$(call require_app,$@)
	@$(MAKE) -C "$(APP_DIR)/$(app)" run

clean:
	@echo "Cleaning $(BUILD_DIR)/*"
	@rm -rf "$(BUILD_DIR)"/* 2>/dev/null || true

delete-app:
	$(call require_app,$@)
	@echo "Deleting app: $(app)"
	@rm -rf "$(APP_DIR)/$(app)" "$(BUILD_DIR)/$(app)" 2>/dev/null || true
	@echo "Deleted: $(app)"
