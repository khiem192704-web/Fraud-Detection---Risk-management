CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

# Core domain & infrastructure sources
SRCS_CORE = sources/core/common/Types.cpp \
            sources/core/domain/transaction.cpp \
            sources/core/domain/device.cpp \
            sources/core/domain/location.cpp \
            sources/core/domain/FraudAlert.cpp \
            sources/core/domain/RiskAssessment.cpp \
            sources/core/domain/Customer.cpp

# Engine & Rules implementation
SRCS_ENGINE = sources/engine/rules/DeviceRiskRule.cpp \
              sources/engine/rules/HighAmountRule.cpp \
              sources/engine/rules/BlacklistRule.cpp \
              sources/engine/rules/VelocityRule.cpp \
              sources/engine/NativeMLPredictor.cpp \
              sources/engine/RiskEngine.cpp \
              sources/engine/DecisionEngine.cpp

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