import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

# Set font family
plt.rcParams['font.sans-serif'] = 'DejaVu Sans'
plt.rcParams['font.family'] = 'sans-serif'

# ==============================================================================
# DIAGRAM 1: LAYERED ARCHITECTURE & FILE RELATIONSHIPS
# ==============================================================================
def create_architecture_diagram():
    fig, ax = plt.subplots(figsize=(16, 11), dpi=300)
    ax.set_xlim(0, 16)
    ax.set_ylim(0, 11)
    ax.axis('off')

    # Background
    fig.patch.set_facecolor('#F8FAFC')
    ax.set_facecolor('#F8FAFC')

    # Title
    ax.text(8, 10.5, "HỆ THỐNG PHÁT HIỆN GIAN LẬN & QUẢN TRỊ RỦI RO (EPFD-RAS)", 
            fontsize=16, fontweight='bold', ha='center', va='center', color='#0F172A')
    ax.text(8, 10.15, "SƠ ĐỒ KIẾN TRÚC PHÂN TẦNG VÀ MỐI QUAN HỆ CÁC TẬP TIN (LAYERED ARCHITECTURE)", 
            fontsize=11, fontstyle='italic', ha='center', va='center', color='#475569')

    # Function to draw layers
    def draw_layer_box(x, y, w, h, title, subtitle, bg_color, border_color):
        rect = patches.FancyBboxPatch((x, y), w, h, boxstyle="round,pad=0.1,rounding_size=0.15",
                                      facecolor=bg_color, edgecolor=border_color, linewidth=2, alpha=0.95)
        ax.add_patch(rect)
        ax.text(x + 0.3, y + h - 0.35, title, fontsize=11, fontweight='bold', color=border_color, va='center')
        ax.text(x + 0.3, y + h - 0.7, subtitle, fontsize=8.5, fontstyle='italic', color='#64748B', va='center')

    # Function to draw file cards
    def draw_file_card(x, y, w, h, name, desc, file_type="Header + Source", card_bg='#FFFFFF', card_border='#CBD5E1', header_color='#1E293B'):
        rect = patches.FancyBboxPatch((x, y), w, h, boxstyle="round,pad=0.05,rounding_size=0.08",
                                      facecolor=card_bg, edgecolor=card_border, linewidth=1.2, zorder=3)
        ax.add_patch(rect)
        # Header bar
        header_rect = patches.FancyBboxPatch((x, y + h - 0.35), w, 0.35, boxstyle="round,pad=0.02,rounding_size=0.05",
                                             facecolor=header_color, edgecolor=header_color, linewidth=0, zorder=4)
        ax.add_patch(header_rect)
        ax.text(x + w/2, y + h - 0.18, name, fontsize=8.5, fontweight='bold', color='#FFFFFF', ha='center', va='center', zorder=5)
        ax.text(x + 0.15, y + h - 0.55, f"[{file_type}]", fontsize=7, color='#64748B', va='center', zorder=5)
        # Description lines
        lines = desc.split('\n')
        curr_y = y + h - 0.85
        for l in lines:
            ax.text(x + 0.15, curr_y, l, fontsize=7.2, color='#334155', va='center', zorder=5)
            curr_y -= 0.28

    # LAYER 1: Presentation & Services Layer (Top)
    draw_layer_box(0.8, 7.6, 14.4, 2.1, "1. PRESENTATION & SERVICES LAYER", "Tầng giao diện, điều phối ứng dụng & xác thực người dùng", "#EFF6FF", "#2563EB")
    draw_file_card(1.2, 7.8, 4.0, 1.2, "sources/main.cpp", "• Điểm khởi chạy ứng dụng (Entry Point)\n• Khởi tạo quy trình Demo & Benchmark\n• Điều phối luồng xử lý toàn hệ thống", "Application Root", "#FFFFFF", "#93C5FD", "#1D4ED8")
    draw_file_card(5.6, 7.8, 4.4, 1.2, "Auth.h / Auth.cpp", "• Quản lý thông tin quản trị viên (AdminUser)\n• Xác thực đăng nhập, phân quyền hệ thống\n• Lưu trữ phiên làm việc an toàn", "include/services", "#FFFFFF", "#93C5FD", "#1D4ED8")
    draw_file_card(10.4, 7.8, 4.4, 1.2, "ConsoleUI / Showcase", "• Hiển thị kết quả đánh giá rủi ro trực quan\n• Màu sắc ANSI (Xanh: Pass, Đỏ: Block)\n• Giải thích nguyên nhân gian lận", "Presentation", "#FFFFFF", "#93C5FD", "#1D4ED8")

    # LAYER 2: Engine & Rules Layer (Mid-Upper)
    draw_layer_box(0.8, 5.0, 14.4, 2.3, "2. RISK ENGINE & DECISION LAYER", "Động cơ thẩm định rủi ro đa nhân tố (Hybrid Rules + ML) & Phân luồng quyết định", "#F0FDFA", "#0D9488")
    draw_file_card(1.2, 5.2, 3.4, 1.35, "RiskEngine.h / .cpp", "• Thu nạp danh sách luật Vector<IFraudRule*>\n• Tổng hợp điểm rủi ro: Policy (0-100)\n• Trả về đối tượng RiskAssessment", "include/engine", "#FFFFFF", "#99F6E4", "#0F766E")
    draw_file_card(4.9, 5.2, 3.3, 1.35, "DecisionEngine.h / .cpp", "• Quyết định hành động: APPROVE/BLOCK\n• Xử lý chặn khẩn cấp (Emergency Block)\n• Cập nhật trạng thái giao dịch", "include/engine", "#FFFFFF", "#99F6E4", "#0F766E")
    draw_file_card(8.5, 5.2, 3.2, 1.35, "NativeMLPredictor.h / .cpp", "• Mô hình Machine Learning nhẹ trong C++\n• Nạp trọng số weights.json từ Python\n• Tính xác suất gian lận bằng hàm Sigmoid", "include/engine", "#FFFFFF", "#99F6E4", "#0F766E")
    draw_file_card(12.0, 5.2, 3.0, 1.35, "rules/*.h & *.cpp", "• IFraudRule (Strategy Pattern Interface)\n• VelocityRule, HighAmountRule\n• DeviceRiskRule, BlacklistRule", "include/engine/rules", "#FFFFFF", "#A7F3D0", "#047857")

    # LAYER 3: Domain Entities Layer (Mid-Lower)
    draw_layer_box(0.8, 2.6, 14.4, 2.1, "3. DOMAIN MODEL LAYER (ENTITIES & VALUE OBJECTS)", "Các thực thể nghiệp vụ cốt lõi, bảo vệ tính bất biến và chuẩn thanh toán quốc tế PCI-DSS", "#FEF2F2", "#DC2626")
    draw_file_card(1.2, 2.8, 2.6, 1.2, "Transaction.h / .cpp", "• Thực thể giao dịch trung tâm\n• Quản lý trạng thái (State)\n• Chứa Device & Location", "Domain Entity", "#FFFFFF", "#FECACA", "#B91C1C")
    draw_file_card(4.1, 2.8, 2.6, 1.2, "Customer.h / .cpp", "• Thông tin khách hàng & KYC\n• Baseline risk score\n• Quản lý thiết bị & tài khoản", "Domain Entity", "#FFFFFF", "#FECACA", "#B91C1C")
    draw_file_card(7.0, 2.8, 2.6, 1.2, "Account.h / .cpp", "• Số dư tiền tệ (Balance)\n• Thao tác nạp/rút tiền an toàn\n• Đóng băng tài khoản (Freeze)", "Domain Entity", "#FFFFFF", "#FECACA", "#B91C1C")
    draw_file_card(9.9, 2.8, 2.6, 1.2, "PaymentMethod.h / .cpp", "• Thuật toán kiểm tra thẻ Luhn\n• Che số thẻ PAN (PCI-DSS)\n• Factory: Card, Bank, Wallet", "Domain Entity", "#FFFFFF", "#FECACA", "#B91C1C")
    draw_file_card(12.8, 2.8, 2.2, 1.2, "Types.h / Types.cpp", "• Enum: Status, RiskLevel...\n• Chuyển đổi chuỗi toString()\n• Các hằng số toàn hệ thống", "Common Types", "#FFFFFF", "#FECACA", "#B91C1C")

    # LAYER 4: Storage & Handcrafted DSA Layer (Bottom)
    draw_layer_box(0.8, 0.4, 14.4, 1.9, "4. STORAGE & HANDCRAFTED DSA LAYER", "Trừu tượng hóa lưu trữ (Repository Pattern) & Thư viện cấu trúc dữ liệu tự viết tay O(1)", "#FFFBEB", "#D97706")
    draw_file_card(1.2, 0.6, 4.4, 1.15, "IRepository.h (Template Interface)", "• CRUD Interface thuần ảo: add(), findById(), getAll(), update(), remove()\n• Hiện thực: AccountRepo.h/.cpp, CustomerRepo.h/.cpp (Lưu trữ file)", "include/storage", "#FFFFFF", "#FDE68A", "#B45309")
    draw_file_card(6.0, 0.6, 4.4, 1.15, "Deque.h (Sliding Window O(1))", "• Bộ đệm vòng hai đầu (Double-ended queue) tự viết tay\n• Sử dụng trong VelocityRule để kiểm tra tần suất quẹt thẻ dồn dập", "include/DSA", "#FFFFFF", "#FDE68A", "#B45309")
    draw_file_card(10.8, 0.6, 4.2, 1.15, "Vector.h & PriorityQueue.h", "• Vector<T>: Mảng động tự co giãn bộ nhớ\n• PriorityQueue<T>: Max-Heap lọc Top-K rủi ro cao nhất cho điều tra viên", "include/DSA", "#FFFFFF", "#FDE68A", "#B45309")

    # Inter-layer Connection Arrows
    def draw_arrow(x1, y1, x2, y2, label=""):
        ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                    arrowprops=dict(arrowstyle="->", color="#475569", lw=1.5, ls="--", connectionstyle="arc3,rad=0.0"))
        if label:
            ax.text((x1+x2)/2 + 0.15, (y1+y2)/2, label, fontsize=7.5, color="#1E293B", fontweight="bold", backgroundcolor="#FFFFFF")

    draw_arrow(3.2, 7.8, 3.2, 6.55, "Gọi điều phối")
    draw_arrow(7.8, 7.8, 3.2, 6.55)
    draw_arrow(2.8, 5.2, 2.5, 4.0, "Đánh giá tx")
    draw_arrow(6.5, 5.2, 2.5, 4.0, "Cập nhật status")
    draw_arrow(13.5, 5.2, 8.2, 1.75, "Sliding Window")
    draw_arrow(2.5, 2.8, 2.5, 1.75, "Lưu trữ Account")

    plt.tight_layout()
    fig.savefig("docs/diagrams/diagram1_architecture.png", dpi=300, bbox_inches='tight')
    plt.close(fig)
    print("Generated: docs/diagrams/diagram1_architecture.png")

# ==============================================================================
# DIAGRAM 2: DETAILED UML CLASS DIAGRAM
# ==============================================================================
def create_uml_class_diagram():
    fig, ax = plt.subplots(figsize=(20, 14), dpi=300)
    ax.set_xlim(0, 20)
    ax.set_ylim(0, 14)
    ax.axis('off')

    fig.patch.set_facecolor('#F8FAFC')
    ax.set_facecolor('#F8FAFC')

    # Title
    ax.text(10, 13.6, "SƠ ĐỒ LỚP CHI TIẾT THIẾT KẾ HƯỚNG ĐỐI TƯỢNG (UML CLASS DIAGRAM)", 
            fontsize=17, fontweight='bold', ha='center', va='center', color='#0F172A')
    ax.text(10, 13.25, "Hệ Thống EPFD-RAS: Quan hệ Kế thừa, Hiện thực, Hợp thành, Thu nạp và Phụ thuộc", 
            fontsize=11.5, fontstyle='italic', ha='center', va='center', color='#475569')

    # Function to draw UML Class box
    def draw_uml_class(x, y, w, h, name, stereotype="", attrs=[], methods=[], 
                       header_color='#1E3A8A', bg_color='#FFFFFF', border_color='#94A3B8'):
        # Outer box
        rect = patches.FancyBboxPatch((x, y), w, h, boxstyle="square,pad=0",
                                      facecolor=bg_color, edgecolor=border_color, linewidth=1.5, zorder=2)
        ax.add_patch(rect)
        
        # Header box
        header_h = 0.55 if not stereotype else 0.75
        h_rect = patches.Rectangle((x, y + h - header_h), w, header_h,
                                   facecolor=header_color, edgecolor=border_color, linewidth=1.5, zorder=3)
        ax.add_patch(h_rect)
        
        if stereotype:
            ax.text(x + w/2, y + h - 0.22, stereotype, fontsize=7.5, fontstyle='italic', color='#E2E8F0', ha='center', va='center', zorder=4)
            ax.text(x + w/2, y + h - 0.52, name, fontsize=9.5, fontweight='bold', color='#FFFFFF', ha='center', va='center', zorder=4)
        else:
            ax.text(x + w/2, y + h - header_h/2, name, fontsize=10, fontweight='bold', color='#FFFFFF', ha='center', va='center', zorder=4)

        # Attributes section
        curr_y = y + h - header_h - 0.25
        for attr in attrs:
            ax.text(x + 0.12, curr_y, attr, fontsize=7.5, color='#1E293B', va='center', zorder=4)
            curr_y -= 0.23
            
        # Divider line
        div_y = curr_y - 0.05
        ax.plot([x, x + w], [div_y, div_y], color=border_color, lw=1, zorder=3)
        curr_y = div_y - 0.22

        # Methods section
        for m in methods:
            ax.text(x + 0.12, curr_y, m, fontsize=7.5, color='#0F172A', va='center', zorder=4)
            curr_y -= 0.23

    # ================= 1. STRATEGY PATTERN (TOP RIGHT) =================
    # Interface IFraudRule
    draw_uml_class(13.2, 9.8, 5.8, 2.7, "IFraudRule", "<<interface>>",
                   ["+getId()* : string",
                    "+getName()* : string",
                    "+getCategory()* : FraudRuleCategory",
                    "+getRiskContribution()* : double"],
                   ["+evaluate(tx, cust, out_alert)* : bool",
                    "+~IFraudRule() = default"],
                   header_color='#6D28D9', border_color='#8B5CF6')

    # Concrete Rules
    draw_uml_class(10.2, 6.7, 4.4, 2.2, "HighAmountRule", "",
                   ["-max_amount_threshold: double",
                    "-multiplier_threshold: double",
                    "-risk_contribution: double"],
                   ["+HighAmountRule(max, mul, score)",
                    "+evaluate(...) : bool",
                    "+getRiskContribution() : double"],
                   header_color='#4338CA', border_color='#6366F1')

    draw_uml_class(15.0, 6.7, 4.4, 2.2, "VelocityRule", "",
                   ["-sliding_window: Deque<Timestamp>",
                    "-max_transactions: int",
                    "-window_duration: seconds",
                    "-risk_contribution: double"],
                   ["+VelocityRule(max, dur, score)",
                    "+evaluate(...) : bool",
                    "+reset() : void"],
                   header_color='#4338CA', border_color='#6366F1')

    draw_uml_class(10.2, 4.0, 4.4, 2.2, "DeviceRiskRule", "",
                   ["-risk_contribution: double"],
                   ["+DeviceRiskRule(score)",
                    "+evaluate(...) : bool",
                    "+getRiskContribution() : double"],
                   header_color='#4338CA', border_color='#6366F1')

    draw_uml_class(15.0, 4.0, 4.4, 2.2, "BlacklistRule", "",
                   ["-risk_contribution: double",
                    "-blacklisted_ips: unordered_map",
                    "-blacklisted_accounts: unordered_map"],
                   ["+addBlacklistedIP(ip, reason)",
                    "+isIPBlacklisted(ip) : bool",
                    "+evaluate(...) : bool"],
                   header_color='#4338CA', border_color='#6366F1')

    # ================= 2. ENGINE LAYER (TOP LEFT) =================
    draw_uml_class(0.8, 9.8, 5.4, 2.7, "RiskEngine", "",
                   ["-rules: Vector<IFraudRule*>",
                    "-policy: RiskPolicy"],
                   ["+RiskEngine(custom_policy)",
                    "+~RiskEngine() // Thu hồi rules",
                    "+registerRule(IFraudRule* rule)",
                    "+assessRisk(tx, cust, ml_score) : RiskAssessment",
                    "+getRuleCount() const : int"],
                   header_color='#0F766E', border_color='#14B8A6')

    draw_uml_class(6.6, 9.8, 4.8, 2.7, "DecisionEngine", "",
                   ["-policy: DecisionPolicy"],
                   ["+DecisionEngine(custom_policy)",
                    "+decide(tx, cust, assess) : DecisionAction",
                    "+checkEmergencyBlock(cust, assess) : bool",
                    "+setPolicy(new_policy) : void"],
                   header_color='#0F766E', border_color='#14B8A6')

    draw_uml_class(0.8, 6.7, 4.8, 2.5, "NativeMLPredictor", "",
                   ["-bias: double {-2.8}",
                    "-weights: Vector<double>",
                    "-feature_names: Vector<string>",
                    "-ready: bool"],
                   ["+loadModel(filepath: string) : bool",
                    "+predictProbability(features) : double",
                    "+predictScore(features) : double",
                    "+isReady() const : bool"],
                   header_color='#0F766E', border_color='#14B8A6')

    # ================= 3. DOMAIN ENTITIES (CENTER & BOTTOM) =================
    draw_uml_class(5.8, 4.0, 4.0, 2.4, "RiskAssessment", "",
                   ["-assessment_id: string",
                    "-rule_score, ml_score: double",
                    "-combined_score: double",
                    "-risk_level: RiskLevel",
                    "-decision: DecisionAction",
                    "-alerts: vector<FraudAlert>"],
                   ["+addAlert(FraudAlert) : void",
                    "+isApproved() / isBlocked() : bool",
                    "+toString() const : string"],
                   header_color='#B91C1C', border_color='#EF4444')

    draw_uml_class(0.8, 3.5, 4.6, 2.9, "Transaction", "",
                   ["-transactionid: string",
                    "-amount: double",
                    "-type: TransactionType",
                    "-status: TransactionStatus",
                    "-location: Location",
                    "-device: Device",
                    "-payment_method: PaymentType"],
                   ["+approve() / reject() : bool",
                    "+sendToReview() / challenge() : bool",
                    "+isHighValue(threshold) : bool",
                    "+isCrossBorder(country) : bool",
                    "+toString() const : string"],
                   header_color='#B91C1C', border_color='#EF4444')

    draw_uml_class(0.8, 0.4, 4.6, 2.7, "Customer", "",
                   ["-customer_id: string",
                    "-fullname, email: string",
                    "-home_location: Location",
                    "-risk_tier: CustomerRiskTier",
                    "-baseline_risk_score: double",
                    "-linked_account_ids: vector<string>"],
                   ["+addLinkedAccount(account_id)",
                    "+addTrustedDevice(device_id)",
                    "+isDeviceTrusted(device_id) : bool",
                    "+isKycVerified() const : bool"],
                   header_color='#B91C1C', border_color='#EF4444')

    draw_uml_class(5.8, 0.4, 4.0, 2.7, "Account", "",
                   ["-account_id, customer_id: string",
                    "-balance: double",
                    "-currency: string",
                    "-status: AccountStatus"],
                   ["+hasSufficientBalance(amount) : bool",
                    "+deposit(amount) : void",
                    "+withdraw(amount) : bool",
                    "+freeze() / unfreeze() : void",
                    "+isActive() const : bool"],
                   header_color='#B91C1C', border_color='#EF4444')

    # Value Objects
    draw_uml_class(10.2, 0.4, 4.4, 2.8, "PaymentMethod", "",
                   ["-payment_id: string",
                    "-masked_card_number: string",
                    "-card_bin, last4: string",
                    "-type: PaymentType"],
                   ["+validateLuhn(pan) : static bool",
                    "+maskPan(pan) : static string",
                    "+createBankTransfer(...) : static PM",
                    "+createEWallet(...) : static PM",
                    "+isCard() const : bool"],
                   header_color='#B91C1C', border_color='#EF4444')

    draw_uml_class(15.0, 0.4, 4.4, 2.8, "Location & Device", "<<Value Objects>>",
                   ["Location: country, city, lat, lon",
                    "Device: device_id, ip_address",
                    "Device: fingerprint, is_virtual"],
                   ["+Location::distanceTo(other) : double",
                    "+Location::isValid() const : bool",
                    "+Device::is_Safe() const : bool",
                    "+Device::toString() const : string"],
                   header_color='#B91C1C', border_color='#EF4444')

    # ================= RELATIONSHIP CONNECTORS =================
    # Realization: IFraudRule <|.. Rules
    def draw_realization(x1, y1, x2, y2):
        ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                    arrowprops=dict(arrowstyle="-|>", color="#7C3AED", lw=1.8, ls="--", mutation_scale=15))

    draw_realization(12.4, 8.9, 14.5, 9.8) # HighAmountRule -> IFraudRule
    draw_realization(16.5, 8.9, 16.5, 9.8) # VelocityRule -> IFraudRule
    draw_realization(12.4, 6.2, 12.4, 6.7) # DeviceRiskRule -> HighAmountRule branch
    draw_realization(16.5, 6.2, 16.5, 6.7) # BlacklistRule -> VelocityRule branch

    # Aggregation: RiskEngine o-- IFraudRule (Hollow Diamond at RiskEngine)
    ax.annotate('', xy=(13.2, 11.2), xytext=(6.2, 11.2),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=2, ls="-"))
    # Draw hollow diamond at 6.2, 11.2
    diamond = patches.Polygon([[6.2, 11.2], [6.4, 11.35], [6.6, 11.2], [6.4, 11.05]], 
                              closed=True, edgecolor="#0F766E", facecolor="#FFFFFF", lw=2, zorder=5)
    ax.add_patch(diamond)
    ax.text(8.5, 11.35, "Vector<IFraudRule*> (Strategy)", fontsize=8, color="#0F766E", fontweight="bold")

    # Dependency: RiskEngine ..> RiskAssessment
    ax.annotate('', xy=(7.8, 6.4), xytext=(3.5, 9.8),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=1.5, ls="--"))
    ax.text(5.5, 8.2, "tạo ra RiskAssessment", fontsize=8, color="#0F766E")

    # Dependency: DecisionEngine ..> RiskAssessment
    ax.annotate('', xy=(7.8, 6.4), xytext=(8.0, 9.8),
                arrowprops=dict(arrowstyle="->", color="#0F766E", lw=1.5, ls="--"))

    # Composition: Transaction *-- Location, Transaction *-- Device (Filled diamond at Transaction)
    # Legend
    legend_rect = patches.FancyBboxPatch((10.2, 3.2), 9.2, 0.6, boxstyle="round,pad=0.05",
                                          facecolor="#FFFFFF", edgecolor="#94A3B8", lw=1)
    ax.add_patch(legend_rect)
    ax.text(10.4, 3.5, "Ký hiệu UML: ", fontsize=8, fontweight='bold', color="#0F172A")
    ax.text(11.8, 3.5, "- - |> Kế thừa/Hiện thực", fontsize=7.5, color="#7C3AED", fontweight='bold')
    ax.text(14.3, 3.5, "◆-- Hợp thành (Composition)", fontsize=7.5, color="#B91C1C", fontweight='bold')
    ax.text(17.2, 3.5, "◇-- Thu nạp (Aggregation)", fontsize=7.5, color="#0F766E", fontweight='bold')

    plt.tight_layout()
    fig.savefig("docs/diagrams/diagram2_class_diagram.png", dpi=300, bbox_inches='tight')
    plt.close(fig)
    print("Generated: docs/diagrams/diagram2_class_diagram.png")

# ==============================================================================
# DIAGRAM 3: OBJECT INTERACTION & SEQUENCE EXECUTION FLOW
# ==============================================================================
def create_sequence_flow_diagram():
    fig, ax = plt.subplots(figsize=(16, 9), dpi=300)
    ax.set_xlim(0, 16)
    ax.set_ylim(0, 9)
    ax.axis('off')

    fig.patch.set_facecolor('#F8FAFC')
    ax.set_facecolor('#F8FAFC')

    # Title
    ax.text(8, 8.6, "SƠ ĐỒ TƯƠNG TÁC ĐỐI TƯỢNG VÀ LUỒNG DỮ LIỆU THỜI GIAN THỰC", 
            fontsize=15, fontweight='bold', ha='center', va='center', color='#0F172A')
    ax.text(8, 8.25, "Quy trình 6 bước xử lý giao dịch qua Động cơ lai (Strategy Rules + ML + Decision Engine)", 
            fontsize=10.5, fontstyle='italic', ha='center', va='center', color='#475569')

    # 6 Step Nodes
    steps = [
        {"num": "1", "title": "Tiếp nhận & Chuẩn hóa", "obj": "Transaction / PaymentMethod", 
         "desc": "• Kiểm tra tính hợp lệ số thẻ (Luhn)\n• Che số thẻ PAN theo chuẩn PCI-DSS\n• Gắn kết Device & Tọa độ Location", "color": "#2563EB", "x": 1.0, "y": 4.5},
        {"num": "2", "title": "Đánh giá đa luật", "obj": "Vector<IFraudRule*>", 
         "desc": "• Lặp đa hình qua 4 Concrete Rules\n• Velocity: Sliding Window với Deque\n• Blacklist: Tra cứu IP/Account O(1)\n• Sinh FraudAlert nếu vi phạm", "color": "#7C3AED", "x": 6.0, "y": 4.5},
        {"num": "3", "title": "Dự đoán Học Máy", "obj": "NativeMLPredictor", 
         "desc": "• Đọc trọng số từ model_weights.json\n• Vector đặc trưng 4 chiều (Amount...)\n• Tính xác suất P(fraud) bằng hàm Sigmoid\n• Quy đổi thang điểm rủi ro ML 0-100", "color": "#0D9488", "x": 11.0, "y": 4.5},
        {"num": "4", "title": "Tổng hợp Rủi ro", "obj": "RiskEngine / RiskPolicy", 
         "desc": "• Trọng số: 50% Rules + 40% ML + 10% KYC\n• Tính combined_score (0 - 100)\n• Phân tầng RiskLevel (VERY_LOW -> CRITICAL)\n• Đóng gói thành RiskAssessment", "color": "#EA580C", "x": 11.0, "y": 1.2},
        {"num": "5", "title": "Phân luồng Quyết định", "obj": "DecisionEngine", 
         "desc": "• Áp dụng ngưỡng DecisionPolicy:\n  - Score < 40.0: APPROVE (Duyệt GD)\n  - Score < 60.0: CHALLENGE (OTP 3DS)\n  - Score < 80.0: REVIEW (Điều tra)\n  - Score >= 80.0: BLOCK (Chặn tức thì)", "color": "#DC2626", "x": 6.0, "y": 1.2},
        {"num": "6", "title": "Cập nhật & Lưu vết", "obj": "Account / Repositories", 
         "desc": "• Nếu APPROVE: Trừ tiền tài khoản an toàn\n• Nếu BLOCK: Giữ nguyên số dư ví/thẻ\n• Ghi nhật ký Audit Trail bất biến\n• Lưu trạng thái qua AccountRepo", "color": "#16A34A", "x": 1.0, "y": 1.2}
    ]

    for s in steps:
        x, y = s['x'], s['y']
        w, h = 4.0, 2.5
        rect = patches.FancyBboxPatch((x, y), w, h, boxstyle="round,pad=0.1,rounding_size=0.15",
                                      facecolor='#FFFFFF', edgecolor=s['color'], linewidth=2, zorder=2)
        ax.add_patch(rect)
        
        # Badge
        badge = patches.Circle((x + 0.4, y + h - 0.4), 0.28, facecolor=s['color'], edgecolor='none', zorder=3)
        ax.add_patch(badge)
        ax.text(x + 0.4, y + h - 0.4, s['num'], fontsize=10, fontweight='bold', color='#FFFFFF', ha='center', va='center', zorder=4)
        
        ax.text(x + 0.85, y + h - 0.3, s['title'], fontsize=10, fontweight='bold', color=s['color'], va='center', zorder=3)
        ax.text(x + 0.85, y + h - 0.6, s['obj'], fontsize=8, fontstyle='italic', color='#64748B', va='center', zorder=3)
        
        # Divider
        ax.plot([x + 0.2, x + w - 0.2], [y + h - 0.8, y + h - 0.8], color='#E2E8F0', lw=1, zorder=3)
        
        # Description
        lines = s['desc'].split('\n')
        cur_y = y + h - 1.1
        for l in lines:
            ax.text(x + 0.25, cur_y, l, fontsize=7.5, color='#334155', va='center', zorder=3)
            cur_y -= 0.32

    # Flow arrows between steps
    def draw_flow_arrow(x1, y1, x2, y2, text=""):
        ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                    arrowprops=dict(arrowstyle="->", color="#0F172A", lw=2.2, mutation_scale=18))
        if text:
            ax.text((x1+x2)/2, (y1+y2)/2 + 0.2, text, fontsize=8, fontweight='bold', color='#0F172A', ha='center', backgroundcolor='#F8FAFC')

    draw_flow_arrow(5.1, 5.75, 5.9, 5.75, "tx, customer")
    draw_flow_arrow(10.1, 5.75, 10.9, 5.75, "features")
    draw_flow_arrow(13.0, 4.4, 13.0, 3.8, "rules + ml_score")
    draw_flow_arrow(10.9, 2.45, 10.1, 2.45, "RiskAssessment")
    draw_flow_arrow(5.9, 2.45, 5.1, 2.45, "DecisionAction")

    plt.tight_layout()
    fig.savefig("docs/diagrams/diagram3_sequence_flow.png", dpi=300, bbox_inches='tight')
    plt.close(fig)
    print("Generated: docs/diagrams/diagram3_sequence_flow.png")

if __name__ == "__main__":
    create_architecture_diagram()
    create_uml_class_diagram()
    create_sequence_flow_diagram()
