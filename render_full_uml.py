# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

# Set font configuration
plt.rcParams['font.sans-serif'] = 'DejaVu Sans'
plt.rcParams['font.family'] = 'sans-serif'

def render_full_uml():
    # Large canvas for crisp, uncrowded layout
    fig_w, fig_h = 28, 20
    fig, ax = plt.subplots(figsize=(fig_w, fig_h), dpi=300)
    ax.set_xlim(0, fig_w)
    ax.set_ylim(0, fig_h)
    ax.axis('off')

    # Background color
    fig.patch.set_facecolor('#F8FAFC')
    ax.set_facecolor('#F8FAFC')

    # Main Diagram Title
    ax.text(fig_w / 2, fig_h - 0.7, 
            "SƠ ĐỒ THIẾT KẾ HƯỚNG ĐỐI TƯỢNG (UML CLASS DIAGRAM) - HỆ THỐNG EPFD-RAS", 
            fontsize=20, fontweight='bold', ha='center', va='center', color='#0F172A')
    ax.text(fig_w / 2, fig_h - 1.2, 
            "Mối quan hệ chi tiết: Kế thừa (Inheritance), Hiện thực (Realization), Hợp thành (Composition), Thu nạp (Aggregation) và Phụ thuộc (Dependency)", 
            fontsize=12.5, fontstyle='italic', ha='center', va='center', color='#475569')

    # Helper function to draw UML Class boxes
    def draw_class(x, y, w, h, name, stereotype="", attrs=[], methods=[], 
                   header_bg='#1E3A8A', border_c='#64748B', body_bg='#FFFFFF'):
        # Outer box
        rect = patches.FancyBboxPatch((x, y), w, h, boxstyle="square,pad=0",
                                      facecolor=body_bg, edgecolor=border_c, linewidth=1.8, zorder=2)
        ax.add_patch(rect)
        
        # Header box
        header_h = 0.55 if not stereotype else 0.8
        h_rect = patches.Rectangle((x, y + h - header_h), w, header_h,
                                   facecolor=header_bg, edgecolor=border_c, linewidth=1.8, zorder=3)
        ax.add_patch(h_rect)
        
        if stereotype:
            ax.text(x + w/2, y + h - 0.22, stereotype, fontsize=8, fontstyle='italic', color='#E2E8F0', ha='center', va='center', zorder=4)
            ax.text(x + w/2, y + h - 0.55, name, fontsize=10.5, fontweight='bold', color='#FFFFFF', ha='center', va='center', zorder=4)
        else:
            ax.text(x + w/2, y + h - header_h/2, name, fontsize=11, fontweight='bold', color='#FFFFFF', ha='center', va='center', zorder=4)

        # Attributes section
        curr_y = y + h - header_h - 0.25
        for attr in attrs:
            ax.text(x + 0.15, curr_y, attr, fontsize=8.2, color='#1E293B', va='center', zorder=4)
            curr_y -= 0.24
            
        # Divider line
        div_y = curr_y - 0.05
        ax.plot([x, x + w], [div_y, div_y], color=border_c, lw=1.2, zorder=3)
        curr_y = div_y - 0.22

        # Methods section
        for m in methods:
            ax.text(x + 0.15, curr_y, m, fontsize=8.2, color='#0F172A', va='center', zorder=4)
            curr_y -= 0.24

    # Zone group boundaries (Subtle backdrop panels)
    def draw_zone_panel(x, y, w, h, title, border_c, bg_c):
        panel = patches.FancyBboxPatch((x, y), w, h, boxstyle="round,pad=0.1,rounding_size=0.2",
                                      facecolor=bg_c, edgecolor=border_c, linewidth=1.5, ls="--", alpha=0.6, zorder=1)
        ax.add_patch(panel)
        ax.text(x + 0.3, y + h - 0.35, title, fontsize=11, fontweight='bold', color=border_c, va='center', zorder=2)

    # -------------------------------------------------------------------------
    # ZONE 1: STORAGE & HANDCRAFTED DSA (TOP LEFT)
    # -------------------------------------------------------------------------
    draw_zone_panel(0.8, 12.0, 6.6, 6.3, "1. STORAGE & HANDCRAFTED DSA LAYER", "#D97706", "#FFFBEB")
    
    draw_class(1.2, 15.3, 2.7, 2.4, "Vector<T>", "<<handcrafted dsa>>",
               ["-data: T*", "-size_: int", "-capacity_: int"],
               ["+push_back(val): void", "+size() const: int", "+operator[](idx): T&", "+reserve(new_cap): void"],
               header_bg='#B45309', border_c='#D97706')

    draw_class(4.3, 15.3, 2.8, 2.4, "Deque<T>", "<<handcrafted dsa>>",
               ["-data: T*", "-capacity: int", "-frontIndex: int", "-count: int"],
               ["+push_back(val): void", "+pop_front(): void", "+front() const: T", "+size() const: int"],
               header_bg='#B45309', border_c='#D97706')

    draw_class(1.2, 12.3, 5.9, 2.5, "IRepository<T>", "<<interface>>",
               [],
               ["+add(entity: const T&)*: bool",
                "+findById(id: const string&)*: T*",
                "+getAll()*: const vector<T>&",
                "+update(entity: const T&)*: bool",
                "+remove(id: const string&)*: bool",
                "+~IRepository() = default"],
               header_bg='#B45309', border_c='#D97706')

    # Concrete Repositories (Mid-Left)
    draw_class(1.2, 9.0, 3.0, 2.4, "AccountRepo", "",
               ["-accounts: vector<Account>", "-file_path: string"],
               ["+loadFromFile(path): bool",
                "+saveToFile(path): bool",
                "+freezeAccount(id): bool",
                "+findByCustomerId(cid): vec"],
               header_bg='#92400E', border_c='#B45309')

    draw_class(4.5, 9.0, 3.0, 2.4, "CustomerRepo", "",
               ["-customers: vector<Customer>", "-file_path: string"],
               ["+loadFromFile(path): bool",
                "+saveToFile(path): bool",
                "+count() const: int",
                "+clear(): void"],
               header_bg='#92400E', border_c='#B45309')

    # -------------------------------------------------------------------------
    # ZONE 2: ENGINE & DECISION LAYER (TOP CENTER)
    # -------------------------------------------------------------------------
    draw_zone_panel(8.0, 11.5, 10.6, 6.8, "2. RISK & DECISION ENGINE LAYER", "#0D9488", "#F0FDFA")

    draw_class(8.4, 15.6, 4.4, 2.3, "RiskPolicy", "<<struct>>",
               ["+weight_rules: double {0.5}",
                "+weight_ml: double {0.4}",
                "+weight_profile: double {0.1}",
                "+threshold_very_low: double {20}",
                "+threshold_high: double {80}"],
               ["+determineRiskLevel(score): RiskLevel"],
               header_bg='#0F766E', border_c='#14B8A6')

    draw_class(13.6, 15.6, 4.6, 2.3, "DecisionPolicy", "<<struct>>",
               ["+approve_max_score: double {39.99}",
                "+challenge_max_score: double {59.99}",
                "+review_max_score: double {79.99}"],
               ["// Score >= 80.0 -> BLOCK"],
               header_bg='#0F766E', border_c='#14B8A6')

    draw_class(8.4, 11.9, 4.8, 3.2, "RiskEngine", "",
               ["-rules: Vector<IFraudRule*>",
                "-policy: RiskPolicy"],
               ["+RiskEngine(custom_policy)",
                "+~RiskEngine() // Thu hồi rules",
                "+registerRule(rule: IFraudRule*): void",
                "+assessRisk(tx, cust, ml_score): RiskAssessment",
                "+getRuleCount() const: int",
                "+setPolicy(new_policy): void"],
               header_bg='#0F766E', border_c='#14B8A6')

    draw_class(13.6, 11.9, 4.6, 3.2, "DecisionEngine", "",
               ["-policy: DecisionPolicy"],
               ["+DecisionEngine(custom_policy)",
                "+decide(tx, cust, assess): DecisionAction",
                "+checkEmergencyBlock(cust, assess): bool",
                "+setPolicy(new_policy): void",
                "+getPolicy() const: const DecisionPolicy&"],
               header_bg='#0F766E', border_c='#14B8A6')

    # Native ML Predictor (Mid-Center)
    draw_class(8.4, 8.4, 4.8, 2.8, "NativeMLPredictor", "",
               ["-bias: double {-2.8}",
                "-weights: Vector<double>",
                "-feature_names: Vector<string>",
                "-model_name: string",
                "-ready: bool {true}"],
               ["+loadModel(filepath: string): bool",
                "+predictProbability(features): double",
                "+predictScore(features): double",
                "+isReady() const: bool"],
               header_bg='#0F766E', border_c='#14B8A6')

    # -------------------------------------------------------------------------
    # ZONE 3: STRATEGY PATTERN FRAUD RULES (TOP RIGHT)
    # -------------------------------------------------------------------------
    draw_zone_panel(19.2, 8.0, 8.2, 10.3, "3. STRATEGY PATTERN: FRAUD RULES", "#7C3AED", "#FAF5FF")

    draw_class(19.6, 15.0, 7.4, 2.9, "IFraudRule", "<<interface>>",
               [],
               ["+getId()* const: string",
                "+getName()* const: string",
                "+getCategory()* const: FraudRuleCategory",
                "+getRiskContribution()* const: double",
                "+evaluate(tx, customer, out_alert)*: bool",
                "+~IFraudRule() = default"],
               header_bg='#6D28D9', border_c='#8B5CF6')

    draw_class(19.6, 11.6, 3.5, 2.8, "HighAmountRule", "",
               ["-max_amount_threshold: double",
                "-multiplier_threshold: double",
                "-risk_contribution: double"],
               ["+HighAmountRule(max, mul, score)",
                "+evaluate(tx, cust, alert): bool",
                "+getId() const: string",
                "+getRiskContribution(): double"],
               header_bg='#5B21B6', border_c='#7C3AED')

    draw_class(23.5, 11.6, 3.5, 2.8, "VelocityRule", "",
               ["-sliding_window: Deque<Timestamp>",
                "-max_transactions: int",
                "-window_duration: seconds",
                "-risk_contribution: double"],
               ["+VelocityRule(max, dur, score)",
                "+evaluate(tx, cust, alert): bool",
                "+reset(): void"],
               header_bg='#5B21B6', border_c='#7C3AED')

    draw_class(19.6, 8.4, 3.5, 2.7, "DeviceRiskRule", "",
               ["-risk_contribution: double"],
               ["+DeviceRiskRule(score)",
                "+evaluate(tx, cust, alert): bool",
                "+getRiskContribution(): double"],
               header_bg='#5B21B6', border_c='#7C3AED')

    draw_class(23.5, 8.4, 3.5, 2.7, "BlacklistRule", "",
               ["-risk_contribution: double",
                "-blacklisted_ips: unordered_map",
                "-blacklisted_accounts: unordered_map"],
               ["+addBlacklistedIP(ip, reason)",
                "+isIPBlacklisted(ip) const: bool",
                "+evaluate(tx, cust, alert): bool"],
               header_bg='#5B21B6', border_c='#7C3AED')

    # -------------------------------------------------------------------------
    # ZONE 4: DOMAIN MODEL LAYER (BOTTOM)
    # -------------------------------------------------------------------------
    draw_zone_panel(0.8, 0.6, 18.0, 7.3, "4. DOMAIN ENTITY & VALUE OBJECT LAYER", "#DC2626", "#FEF2F2")

    draw_class(1.2, 4.3, 4.0, 3.2, "Location", "<<value object>>",
               ["-country: string",
                "-city: string",
                "-latitude: double",
                "-longitude: double"],
               ["+isValid() const: bool",
                "+distanceTo(other: Location): double",
                "+setCountry(country): void",
                "+toString() const: string"],
               header_bg='#B91C1C', border_c='#EF4444')

    draw_class(5.6, 4.3, 4.2, 3.2, "Device", "<<entity>>",
               ["-device_id: string",
                "-ip_address: string",
                "-device_fingerprint: string",
                "-location: Location",
                "-is_virtual: bool",
                "-is_hacker: bool"],
               ["+is_Safe() const: bool",
                "+GetLocation() const: const Location&",
                "+toString() const: string"],
               header_bg='#B91C1C', border_c='#EF4444')

    draw_class(10.2, 4.1, 4.6, 3.4, "Account", "<<entity>>",
               ["-account_id: string",
                "-customer_id: string",
                "-balance: double",
                "-currency: string",
                "-status: AccountStatus",
                "-created_at: Timestamp"],
               ["+hasSufficientBalance(amt): bool",
                "+deposit(amt): void",
                "+withdraw(amt): bool",
                "+freeze() / unfreeze(): void",
                "+isActive() / isFrozen(): bool"],
               header_bg='#B91C1C', border_c='#EF4444')

    draw_class(1.2, 0.9, 5.0, 3.0, "Customer", "<<entity>>",
               ["-customer_id: string",
                "-fullname, email, phone: string",
                "-home_location: Location",
                "-risk_tier: CustomerRiskTier",
                "-baseline_risk_score: double",
                "-is_kyc_verified: bool",
                "-linked_account_ids: vector<string>",
                "-trusted_device_ids: vector<string>"],
               ["+addLinkedAccount(account_id): void",
                "+addTrustedDevice(device_id): void",
                "+isDeviceTrusted(device_id) const: bool",
                "+isKycVerified() const: bool"],
               header_bg='#B91C1C', border_c='#EF4444')

    draw_class(6.6, 0.9, 5.0, 3.0, "Transaction", "<<aggregate root entity>>",
               ["-transactionid: string",
                "-type: TransactionType",
                "-customerid, recipientid, accountid: string",
                "-amount: double",
                "-location: Location",
                "-device: Device",
                "-payment_method: PaymentType",
                "-status: TransactionStatus"],
               ["+approve() / reject() / sendToReview(): bool",
                "+challenge() / settle() / chargeback(): bool",
                "+isHighValue(threshold) const: bool",
                "+isCrossBorder(country) const: bool",
                "+toString() const: string"],
               header_bg='#B91C1C', border_c='#EF4444')

    draw_class(12.0, 0.9, 4.2, 3.0, "PaymentMethod", "<<entity / helper>>",
               ["-payment_id: string",
                "-type: PaymentType",
                "-masked_card_number: string",
                "-card_bin, last4: string",
                "-card_holder_name: string",
                "-expiry_month, expiry_year: int"],
               ["+createBankTransfer(...): static PM",
                "+createEWallet(...): static PM",
                "+validateLuhn(pan): static bool",
                "+maskPan(pan): static string",
                "+isCard() const: bool"],
               header_bg='#B91C1C', border_c='#EF4444')

    # -------------------------------------------------------------------------
    # ZONE 5: ASSESSMENTS, ALERTS & TYPES (BOTTOM RIGHT)
    # -------------------------------------------------------------------------
    draw_zone_panel(19.2, 0.6, 8.2, 7.0, "5. RISK ASSESSMENT & COMMON TYPES", "#475569", "#F1F5F9")

    draw_class(19.6, 4.0, 3.8, 3.2, "FraudAlert", "<<entity>>",
               ["-alert_id: string",
                "-transaction_id: string",
                "-rule_id: string",
                "-rule_name: string",
                "-category: FraudRuleCategory",
                "-score_contribution: double",
                "-reason: string",
                "-severity: RiskLevel"],
               ["+isCritical() const: bool",
                "+getRiskContribution() const: double",
                "+getSeverity() const: RiskLevel",
                "+toString() const: string"],
               header_bg='#475569', border_c='#64748B')

    draw_class(23.8, 4.0, 3.4, 3.2, "Types", "<<enums & helpers>>",
               ["+TransactionType",
                "+TransactionStatus",
                "+PaymentType",
                "+RiskLevel",
                "+DecisionAction",
                "+CustomerRiskTier",
                "+FraudRuleCategory",
                "+AccountStatus"],
               ["+toString(enum): view",
                "+operator<<(os, enum): os&"],
               header_bg='#475569', border_c='#64748B')

    draw_class(19.6, 0.9, 7.4, 2.7, "RiskAssessment", "<<entity>>",
               ["-assessment_id: string",
                "-transaction_id: string",
                "-rule_score, ml_score, combined_score: double",
                "-risk_level: RiskLevel",
                "-decision: DecisionAction",
                "-alerts: vector<FraudAlert>",
                "-reasons: vector<string>"],
               ["+addAlert(alert: const FraudAlert&): void",
                "+addReason(reason: const string&): void",
                "+isApproved() / isReview() / isChallenge() / isBlocked(): bool",
                "+getCombinedScore() const: double",
                "+toString() const: string"],
               header_bg='#475569', border_c='#64748B')

    # =========================================================================
    # CONNECTOR ARROWS & RELATIONSHIPS
    # =========================================================================

    # 1. REALIZATION (Realization / Implementation): IRepository <|.. AccountRepo / CustomerRepo
    def draw_realization(x1, y1, x2, y2):
        ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                    arrowprops=dict(arrowstyle="-|>", color="#D97706", lw=1.8, ls="--", mutation_scale=15))

    draw_realization(2.7, 11.4, 2.7, 12.3)
    draw_realization(5.5, 11.4, 5.5, 12.3)

    # 2. REALIZATION: IFraudRule <|.. 4 Concrete Rules
    def draw_rule_realization(x1, y1, x2, y2):
        ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                    arrowprops=dict(arrowstyle="-|>", color="#7C3AED", lw=1.8, ls="--", mutation_scale=15))

    draw_rule_realization(21.3, 14.4, 21.3, 15.0)
    draw_rule_realization(25.2, 14.4, 25.2, 15.0)
    draw_rule_realization(21.3, 11.1, 21.3, 11.6)
    draw_rule_realization(25.2, 11.1, 25.2, 11.6)

    # 3. COMPOSITION (Filled Diamond): VelocityRule *-- Deque
    ax.annotate('', xy=(7.1, 16.5), xytext=(23.5, 13.0),
                arrowprops=dict(arrowstyle="->", color="#5B21B6", lw=1.8, ls="-"))
    # Diamond at VelocityRule (23.5, 13.0)
    d_v = patches.Polygon([[23.5, 13.0], [23.7, 13.15], [23.9, 13.0], [23.7, 12.85]], 
                          closed=True, edgecolor="#5B21B6", facecolor="#5B21B6", lw=1.5, zorder=5)
    ax.add_patch(d_v)
    ax.text(16.5, 14.5, "sliding_window: Deque<Timestamp>", fontsize=8, color="#5B21B6", fontweight='bold', backgroundcolor="#FAF5FF")

    # 4. COMPOSITION: Transaction *-- Location, Transaction *-- Device
    d_tx_loc = patches.Polygon([[6.6, 2.5], [6.4, 2.65], [6.2, 2.5], [6.4, 2.35]], 
                               closed=True, edgecolor="#B91C1C", facecolor="#B91C1C", lw=1.5, zorder=5)
    ax.add_patch(d_tx_loc)
    ax.annotate('', xy=(5.2, 4.3), xytext=(6.2, 2.5),
                arrowprops=dict(arrowstyle="->", color="#B91C1C", lw=1.8, ls="-"))

    d_tx_dev = patches.Polygon([[8.5, 3.9], [8.35, 4.05], [8.5, 4.2], [8.65, 4.05]], 
                               closed=True, edgecolor="#B91C1C", facecolor="#B91C1C", lw=1.5, zorder=5)
    ax.add_patch(d_tx_dev)
    ax.annotate('', xy=(7.7, 4.3), xytext=(8.5, 3.9),
                arrowprops=dict(arrowstyle="->", color="#B91C1C", lw=1.8, ls="-"))

    # COMPOSITION: Device *-- Location
    d_dev_loc = patches.Polygon([[5.6, 5.8], [5.4, 5.95], [5.2, 5.8], [5.4, 5.65]], 
                                closed=True, edgecolor="#B91C1C", facecolor="#B91C1C", lw=1.5, zorder=5)
    ax.add_patch(d_dev_loc)
    ax.annotate('', xy=(5.2, 5.8), xytext=(5.6, 5.8),
                arrowprops=dict(arrowstyle="->", color="#B91C1C", lw=1.8, ls="-"))

    # COMPOSITION: Customer *-- Location
    d_cust_loc = patches.Polygon([[3.0, 3.9], [2.85, 4.05], [3.0, 4.2], [3.15, 4.05]], 
                                 closed=True, edgecolor="#B91C1C", facecolor="#B91C1C", lw=1.5, zorder=5)
    ax.add_patch(d_cust_loc)
    ax.annotate('', xy=(3.0, 4.3), xytext=(3.0, 3.9),
                arrowprops=dict(arrowstyle="->", color="#B91C1C", lw=1.8, ls="-"))

    # 5. AGGREGATION (Hollow Diamond): RiskEngine o-- IFraudRule*
    ax.annotate('', xy=(19.6, 16.2), xytext=(13.2, 14.5),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=2, ls="-"))
    d_re = patches.Polygon([[13.2, 14.5], [13.4, 14.65], [13.6, 14.5], [13.4, 14.35]], 
                           closed=True, edgecolor="#0F766E", facecolor="#FFFFFF", lw=2, zorder=5)
    ax.add_patch(d_re)
    ax.text(15.2, 15.6, "Vector<IFraudRule*>", fontsize=8.5, color="#0F766E", fontweight='bold', backgroundcolor="#FFFFFF")

    # AGGREGATION: RiskAssessment o-- FraudAlert
    ax.annotate('', xy=(21.5, 4.0), xytext=(21.5, 3.6),
                arrowprops=dict(arrowstyle="->", color="#475569", lw=1.8, ls="-"))
    d_ra = patches.Polygon([[21.5, 3.6], [21.65, 3.45], [21.5, 3.3], [21.35, 3.45]], 
                           closed=True, edgecolor="#475569", facecolor="#FFFFFF", lw=1.8, zorder=5)
    ax.add_patch(d_ra)
    ax.text(21.8, 3.7, "vector<FraudAlert>", fontsize=8, color="#475569", fontweight='bold')

    # AGGREGATION: AccountRepo o-- Account
    ax.annotate('', xy=(10.2, 5.8), xytext=(4.2, 9.5),
                arrowprops=dict(arrowstyle="->", color="#92400E", lw=1.8, ls="-"))
    d_ar = patches.Polygon([[4.2, 9.5], [4.4, 9.65], [4.6, 9.5], [4.4, 9.35]], 
                           closed=True, edgecolor="#92400E", facecolor="#FFFFFF", lw=1.8, zorder=5)
    ax.add_patch(d_ar)
    ax.text(5.5, 8.2, "vector<Account>", fontsize=8, color="#92400E", fontweight='bold')

    # 6. COMPOSITION: RiskEngine *-- RiskPolicy, DecisionEngine *-- DecisionPolicy
    d_rpol = patches.Polygon([[10.6, 15.1], [10.45, 15.25], [10.6, 15.4], [10.75, 15.25]], 
                             closed=True, edgecolor="#0F766E", facecolor="#0F766E", lw=1.5, zorder=5)
    ax.add_patch(d_rpol)
    ax.annotate('', xy=(10.6, 15.6), xytext=(10.6, 15.1),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=1.5, ls="-"))

    d_dpol = patches.Polygon([[15.9, 15.1], [15.75, 15.25], [15.9, 15.4], [16.05, 15.25]], 
                             closed=True, edgecolor="#0F766E", facecolor="#0F766E", lw=1.5, zorder=5)
    ax.add_patch(d_dpol)
    ax.annotate('', xy=(15.9, 15.6), xytext=(15.9, 15.1),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=1.5, ls="-"))

    # 7. DEPENDENCY: RiskEngine ..> Transaction, Customer, RiskAssessment
    ax.annotate('', xy=(11.0, 3.9), xytext=(10.5, 11.9),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=1.5, ls="--"))
    ax.text(10.8, 8.0, "assessRisk(tx, cust)", fontsize=8, color="#0F766E", fontweight='bold', backgroundcolor="#FFFFFF")

    ax.annotate('', xy=(20.5, 3.6), xytext=(13.2, 12.2),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=1.5, ls="--"))
    ax.text(15.5, 7.5, "tạo ra RiskAssessment", fontsize=8, color="#0F766E", backgroundcolor="#FFFFFF")

    # DEPENDENCY: DecisionEngine ..> RiskAssessment, Transaction
    ax.annotate('', xy=(11.6, 3.2), xytext=(15.5, 11.9),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=1.5, ls="--"))
    ax.text(14.0, 6.0, "decide(tx, assess)", fontsize=8, color="#0F766E", fontweight='bold', backgroundcolor="#FFFFFF")

    # DEPENDENCY: RiskEngine ..> NativeMLPredictor
    ax.annotate('', xy=(10.8, 11.2), xytext=(10.8, 11.9),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=1.5, ls="--"))

    # =========================================================================
    # LEGEND BOX (BOTTOM CENTER)
    # =========================================================================
    leg_x, leg_y, leg_w, leg_h = 7.0, 18.5, 14.0, 0.75
    leg_rect = patches.FancyBboxPatch((leg_x, leg_y), leg_w, leg_h, boxstyle="round,pad=0.08",
                                      facecolor="#FFFFFF", edgecolor="#64748B", lw=1.2, zorder=5)
    ax.add_patch(leg_rect)
    ax.text(leg_x + 0.3, leg_y + 0.38, "KÝ HIỆU UML:", fontsize=8.5, fontweight='bold', color="#0F172A", va='center', zorder=6)
    
    # Realization
    ax.annotate('', xy=(leg_x + 3.1, leg_y + 0.38), xytext=(leg_x + 2.3, leg_y + 0.38),
                arrowprops=dict(arrowstyle="-|>", color="#7C3AED", lw=1.6, ls="--", mutation_scale=12))
    ax.text(leg_x + 3.2, leg_y + 0.38, "Hiện thực (Realization)", fontsize=8, color="#7C3AED", va='center', zorder=6)

    # Composition
    d_leg_c = patches.Polygon([[leg_x + 6.3, leg_y + 0.38], [leg_x + 6.45, leg_y + 0.48], [leg_x + 6.6, leg_y + 0.38], [leg_x + 6.45, leg_y + 0.28]], 
                              closed=True, edgecolor="#B91C1C", facecolor="#B91C1C", lw=1.2, zorder=6)
    ax.add_patch(d_leg_c)
    ax.plot([leg_x + 6.6, leg_x + 7.1], [leg_y + 0.38, leg_y + 0.38], color="#B91C1C", lw=1.6, zorder=6)
    ax.text(leg_x + 7.2, leg_y + 0.38, "Hợp thành (Composition)", fontsize=8, color="#B91C1C", va='center', zorder=6)

    # Aggregation
    d_leg_a = patches.Polygon([[leg_x + 10.3, leg_y + 0.38], [leg_x + 10.45, leg_y + 0.48], [leg_x + 10.6, leg_y + 0.38], [leg_x + 10.45, leg_y + 0.28]], 
                              closed=True, edgecolor="#0F766E", facecolor="#FFFFFF", lw=1.5, zorder=6)
    ax.add_patch(d_leg_a)
    ax.plot([leg_x + 10.6, leg_x + 11.1], [leg_y + 0.38, leg_y + 0.38], color="#0F766E", lw=1.6, zorder=6)
    ax.text(leg_x + 11.2, leg_y + 0.38, "Thu nạp (Aggregation)", fontsize=8, color="#0F766E", va='center', zorder=6)

    plt.tight_layout()
    output_png = "docs/diagrams/uml_class_diagram_full.png"
    fig.savefig(output_png, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"Full UML Class Diagram generated successfully at: {output_png}")

if __name__ == "__main__":
    render_full_uml()
