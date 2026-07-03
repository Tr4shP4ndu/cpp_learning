# Top-level workspace Make — scaffold, build, run lessons.
APP_DIR      := apps
BUILD_DIR    := build
TEMPLATE_DIR := templates/app

CXX ?= g++
CC  ?= gcc
STD ?= c++23
CSTD ?= c17
BUILD_TYPE ?= Debug
export CXX CC STD CSTD BUILD_TYPE BUILD_DIR

APPS := $(notdir $(wildcard $(APP_DIR)/*))
TEMPLATES := $(wildcard $(TEMPLATE_DIR)/*.tpl)

define require_app
$(if $(app),,$(error Usage: make $(1) app=<name>))
endef

.PHONY: help list app build run run-c clean delete-app

help:
	@echo "Targets:"
	@echo "  make app app=<name> [lang=c]   Scaffold a new lesson"
	@echo "  make list                      List lessons"
	@echo "  make build [app=<name>]        Build one lesson (or all)"
	@echo "  make run app=<name>            Run the C++ binary"
	@echo "  make run-c app=<name>          Run the C binary (if present)"
	@echo "  make clean                     Remove build outputs"
	@echo "  make delete-app app=<name>     Delete a lesson"
	@echo ""
	@echo "Variables: STD=c++98..c++23 (def c++23)  CSTD=c99|c11|c17"
	@echo "           BUILD_TYPE=Debug|Release  CXX=g++|clang++  CC=gcc|clang"

list:
	@printf "%s\n" $(APPS)

app:
	$(call require_app,$@)
	@mkdir -p "$(APP_DIR)/$(app)/src"
	@for tpl in $(TEMPLATES); do \
		out=$$(basename "$$tpl" .tpl); \
		case "$$out" in \
			main.cpp) [ "$(lang)" = "c" ] && continue; dest="$(APP_DIR)/$(app)/src/main.cpp" ;; \
			main.c)   [ "$(lang)" = "c" ] || continue; dest="$(APP_DIR)/$(app)/src/main.c" ;; \
			*)        dest="$(APP_DIR)/$(app)/$$out" ;; \
		esac; \
		sed 's/__APP_NAME__/$(app)/g' "$$tpl" > "$$dest"; \
	done
	@echo "Created app: $(APP_DIR)/$(app)"

build:
ifdef app
	@$(MAKE) -C "$(APP_DIR)/$(app)" build
else
	@for a in $(APPS); do $(MAKE) -C "$(APP_DIR)/$$a" build || exit 1; done
endif

run:
	$(call require_app,$@)
	@$(MAKE) -C "$(APP_DIR)/$(app)" run

run-c:
	$(call require_app,$@)
	@$(MAKE) -C "$(APP_DIR)/$(app)" run-c

clean:
	@echo "Cleaning $(BUILD_DIR)/*"
	@rm -rf "$(BUILD_DIR)"/* 2>/dev/null || true

delete-app:
	$(call require_app,$@)
	@rm -rf "$(APP_DIR)/$(app)" "$(BUILD_DIR)/$(app)" 2>/dev/null || true
	@echo "Deleted: $(app)"
