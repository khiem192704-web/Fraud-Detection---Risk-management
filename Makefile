CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

# Core domain & infrastructure sources (Đã sửa đường dẫn đúng thực tế)
SRCS_CORE = sources/core/common/Types.cpp \
            sources/core/models/transaction.cpp \
            sources/core/models/device.cpp \
            sources/core/models/location.cpp \
            sources/core/models/FraudAlert.cpp \
            sources/core/models/RiskAssessment.cpp \
            sources/core/models/Customer.cpp \
            sources/core/models/Account.cpp \
            sources/core/models/Merchant.cpp \
            sources/core/models/PaymentMethod.cpp

# Engine & Rules implementation
SRCS_ENGINE = sources/engine/rules/DeviceRiskRule.cpp \
              sources/engine/rules/HighAmountRule.cpp \
              sources/engine/rules/BlacklistRule.cpp \
              sources/engine/rules/VelocityRule.cpp \
              sources/engine/NativeMLPredictor.cpp \
              sources/engine/FeatureExtractor.cpp \
              sources/engine/RiskEngine.cpp \
              sources/engine/DecisionEngine.cpp

# DAL (Data Access Layer)
SRCS_DAL = sources/dal/AccountDAL.cpp \
           sources/dal/CustomerDAL.cpp

# BLL (Business Logic Layer)
SRCS_BLL = sources/bll/Auth.cpp \
           sources/bll/TransactionBLL.cpp

# Main application
SRCS_APP = sources/main.cpp $(SRCS_ENGINE) $(SRCS_BLL) $(SRCS_DAL) $(SRCS_CORE)

# Step-by-step test
SRCS_TEST = tests/test_rules_step_by_step.cpp $(SRCS_ENGINE) $(SRCS_BLL) $(SRCS_DAL) $(SRCS_CORE)

BIN_DIR = bin

TARGET_APP = $(BIN_DIR)/epfd_app.exe
TARGET_TEST = $(BIN_DIR)/test_rules_step.exe

.PHONY: all clean build test run

all: build test

build: $(TARGET_APP) $(TARGET_TEST)

$(BIN_DIR):
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

$(TARGET_APP): $(SRCS_APP) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS_APP)

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