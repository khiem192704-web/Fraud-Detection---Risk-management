CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

# Core domain & infrastructure sources
SRCS_CORE = sources/core/common/Types.cpp \
            sources/core/domain/transaction/transaction.cpp \
            sources/core/domain/device/device.cpp \
            sources/core/domain/location/location.cpp \
            sources/core/domain/alert/FraudAlert.cpp \
            sources/core/domain/alert/RiskAssessment.cpp \
            sources/core/domain/account/Customer.cpp

# Engine & Rules implementation (them dan vao day khi ban code xong tung rule)
SRCS_ENGINE = sources/engine/rules/DeviceRiskRule.cpp

# Main application
SRCS_APP = sources/main.cpp

# Step-by-step test
SRCS_TEST = tests/test_rules_step_by_step.cpp $(SRCS_ENGINE) $(SRCS_CORE)

BIN_DIR = bin

TARGET_APP = $(BIN_DIR)/epfd_app.exe
TARGET_TEST = $(BIN_DIR)/test_rules_step.exe

.PHONY: all clean build test run

all: build test

build: $(TARGET_APP) $(TARGET_TEST)

$(BIN_DIR):
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

$(TARGET_APP): $(SRCS_APP) $(SRCS_CORE) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS_APP) $(SRCS_CORE)

$(TARGET_TEST): $(SRCS_TEST) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS_TEST)

test: $(TARGET_TEST)
	@echo.
	@$(TARGET_TEST)

run: $(TARGET_APP)
	@echo.
	@$(TARGET_APP)

clean:
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"

	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"