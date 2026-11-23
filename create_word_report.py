# -*- coding: utf-8 -*-
import os
import docx
from docx import Document
from docx.shared import Inches, Pt, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT, WD_ALIGN_VERTICAL
from docx.oxml import OxmlElement, parse_xml
from docx.oxml.ns import nsdecls, qn

def set_cell_background(cell, fill_hex):
    """Set background color for a table cell."""
    tcPr = cell._tc.get_or_add_tcPr()
    shd = parse_xml(f'<w:shd {nsdecls("w")} w:fill="{fill_hex}"/>')
    tcPr.append(shd)

def set_cell_margins(cell, top=120, bottom=120, left=150, right=150):
    """Set inner padding for table cell (in dxa: 20 dxa = 1 pt)."""
    tcPr = cell._tc.get_or_add_tcPr()
    tcMar = parse_xml(f'''
        <w:tcMar {nsdecls("w")}>
            <w:top w:w="{top}" w:type="dxa"/>
            <w:bottom w:w="{bottom}" w:type="dxa"/>
            <w:left w:w="{left}" w:type="dxa"/>
            <w:right w:w="{right}" w:type="dxa"/>
        </w:tcMar>
    ''')
    tcPr.append(tcMar)

def add_callout(doc, text, title="LƯU Ý QUAN TRỌNG TRONG OOP"):
    """Add a professional styled callout box."""
    table = doc.add_table(rows=1, cols=1)
    table.alignment = WD_TABLE_ALIGNMENT.CENTER
    table.autofit = False
    cell = table.cell(0, 0)
    cell.width = Inches(6.5)
    
    # Left border thick, others none
    tcPr = cell._tc.get_or_add_tcPr()
    borders = parse_xml(f'''
        <w:tcBorders {nsdecls("w")}>
            <w:top w:val="none"/>
            <w:left w:val="single" w:sz="36" w:space="0" w:color="1E3A8A"/>
            <w:bottom w:val="none"/>
            <w:right w:val="none"/>
        </w:tcBorders>
    ''')
    tcPr.append(borders)
    set_cell_background(cell, "F1F5F9")
    set_cell_margins(cell, top=140, bottom=140, left=200, right=180)
    
    p = cell.paragraphs[0]
    p.paragraph_format.space_before = Pt(2)
    p.paragraph_format.space_after = Pt(2)
    p.paragraph_format.line_spacing = 1.15
    run_title = p.add_run(f"📌 {title}\n")
    run_title.font.name = "Arial"
    run_title.font.size = Pt(10)
    run_title.font.bold = True
    run_title.font.color.rgb = RGBColor(0x1E, 0x3A, 0x8A)
    
    run_text = p.add_run(text)
    run_text.font.name = "Arial"
    run_text.font.size = Pt(9.5)
    run_text.font.italic = True
    run_text.font.color.rgb = RGBColor(0x33, 0x41, 0x55)
    
    doc.add_paragraph().paragraph_format.space_after = Pt(4)

def format_table(table, col_widths, headers, data):
    """Format table with elegant header and alternating row colors."""
    table.alignment = WD_TABLE_ALIGNMENT.CENTER
    
    # Header Row
    hdr_cells = table.rows[0].cells
    for i, title in enumerate(headers):
        hdr_cells[i].text = title
        set_cell_background(hdr_cells[i], "1E3A8A")
        set_cell_margins(hdr_cells[i], top=140, bottom=140, left=150, right=150)
        p = hdr_cells[i].paragraphs[0]
        p.alignment = WD_ALIGN_PARAGRAPH.CENTER
        for run in p.runs:
            run.font.name = "Arial"
            run.font.size = Pt(9.5)
            run.font.bold = True
            run.font.color.rgb = RGBColor(0xFF, 0xFF, 0xFF)
            
    # Data Rows
    for row_idx, row_data in enumerate(data):
        row_cells = table.add_row().cells
        bg_color = "F8FAFC" if row_idx % 2 == 1 else "FFFFFF"
        for col_idx, text in enumerate(row_data):
            row_cells[col_idx].text = text
            set_cell_background(row_cells[col_idx], bg_color)
            set_cell_margins(row_cells[col_idx], top=100, bottom=100, left=120, right=120)
            p = row_cells[col_idx].paragraphs[0]
            p.paragraph_format.space_after = Pt(2)
            p.paragraph_format.line_spacing = 1.15
            for run in p.runs:
                run.font.name = "Arial"
                run.font.size = Pt(8.5)
                run.font.color.rgb = RGBColor(0x1E, 0x29, 0x3B)
                
    # Set widths
    for row in table.rows:
        for i, w in enumerate(col_widths):
            row.cells[i].width = Inches(w)

def build_document():
    doc = Document()
    
    # Page setup: Standard A4 with 1 inch margins
    sections = doc.sections
    for section in sections:
        section.top_margin = Inches(1)
        section.bottom_margin = Inches(1)
        section.left_margin = Inches(1)
        section.right_margin = Inches(1)

    # Styles
    normal_style = doc.styles['Normal']
    normal_style.font.name = 'Arial'
    normal_style.font.size = Pt(10.5)
    normal_style.font.color.rgb = RGBColor(0x1E, 0x29, 0x3B)
    normal_style.paragraph_format.line_spacing = 1.2
    normal_style.paragraph_format.space_after = Pt(6)

    # ==============================================================================
    # COVER / HEADER
    # ==============================================================================
    p_inst = doc.add_paragraph()
    p_inst.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r_inst = p_inst.add_run("TRƯỜNG ĐẠI HỌC BÁCH KHOA - ĐẠI HỌC ĐÀ NẴNG\nKHOA CÔNG NGHỆ THÔNG TIN\n***")
    r_inst.font.name = "Arial"
    r_inst.font.size = Pt(11)
    r_inst.font.bold = True
    r_inst.font.color.rgb = RGBColor(0x47, 0x55, 0x69)
    p_inst.paragraph_format.space_after = Pt(24)

    p_title = doc.add_paragraph()
    p_title.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r_title = p_title.add_run("BÁO CÁO THIẾT KẾ HƯỚNG ĐỐI TƯỢNG (OOP)\nVÀ KIẾN TRÚC MÔ-ĐUN HỆ THỐNG")
    r_title.font.name = "Arial"
    r_title.font.size = Pt(20)
    r_title.font.bold = True
    r_title.font.color.rgb = RGBColor(0x1E, 0x3A, 0x8A)
    p_title.paragraph_format.space_after = Pt(8)

    p_sub = doc.add_paragraph()
    p_sub.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r_sub = p_sub.add_run("Đề tài: Hệ Thống Phát Hiện Gian Lận & Quản Trị Rủi Ro Thanh Toán Điện Tử (EPFD-RAS)\nHọc phần: PBL 2 — Đồ Án Cơ Sở Lập Trình (OOP & DSA)")
    r_sub.font.name = "Arial"
    r_sub.font.size = Pt(12)
    r_sub.font.italic = True
    r_sub.font.color.rgb = RGBColor(0x0D, 0x94, 0x88)
    p_sub.paragraph_format.space_after = Pt(24)

    # Info table
    info_table = doc.add_table(rows=3, cols=2)
    info_table.alignment = WD_TABLE_ALIGNMENT.CENTER
    col_w = [Inches(2.2), Inches(4.2)]
    info_data = [
        ("Sinh viên thực hiện:", "Hoàng & Khiêm"),
        ("Ngôn ngữ & Nền tảng:", "C++17 (Core Engine & Handcrafted DSA) & Python (Machine Learning)"),
        ("Kiến trúc phần mềm:", "Clean Layered Architecture, Strategy Pattern, Repository Pattern")
    ]
    for r_idx, (lbl, val) in enumerate(info_data):
        cell_lbl, cell_val = info_table.rows[r_idx].cells
        cell_lbl.text = lbl
        cell_val.text = val
        cell_lbl.width = col_w[0]
        cell_val.width = col_w[1]
        set_cell_background(cell_lbl, "F1F5F9")
        set_cell_background(cell_val, "FFFFFF")
        set_cell_margins(cell_lbl, 80, 80, 100, 100)
        set_cell_margins(cell_val, 80, 80, 100, 100)
        cell_lbl.paragraphs[0].runs[0].font.bold = True
        cell_lbl.paragraphs[0].runs[0].font.size = Pt(9.5)
        cell_val.paragraphs[0].runs[0].font.size = Pt(9.5)
    
    doc.add_page_break()

    # ==============================================================================
    # CHƯƠNG 1
    # ==============================================================================
    h1 = doc.add_heading("CHƯƠNG 1: TỔNG QUAN KIẾN TRÚC PHÂN TẦNG VÀ CẤU TRÚC THƯ MỤC", level=1)
    h1.paragraph_format.space_before = Pt(14)
    h1.paragraph_format.space_after = Pt(6)

    doc.add_paragraph(
        "Dự án EPFD-RAS (Electronic Payment Fraud Detection & Risk Management) được thiết kế theo mô hình "
        "Kiến trúc Phân tầng (Clean Layered Architecture), tuân thủ chặt chẽ nguyên lý Phân tách Trách nhiệm "
        "(Separation of Concerns). Toàn bộ hệ sinh thái phần mềm được phân tách rõ ràng thành hai phân vùng: "
        "thư mục khai báo giao diện/hợp đồng (`include/`) và thư mục hiện thực chi tiết thuật toán (`sources/`). "
        "Mô hình này ngăn chặn tối đa sự phụ thuộc vòng (circular dependency) và giảm thiểu thời gian biên dịch mã nguồn C++."
    )

    doc.add_paragraph(
        "Hệ thống bao gồm 4 tầng kiến trúc logic chính:"
    )
    doc.add_paragraph(
        "1. Tầng Trình diễn & Dịch vụ (Presentation & Services Layer): Điểm tiếp nhận dữ liệu, điều phối luồng thực thi (`main.cpp`) và quản lý định danh người vận hành (`Auth.h/.cpp`).\n"
        "2. Tầng Động cơ Thẩm định Rủi ro & Ra Quyết định (Engine & Decision Layer): Trái tim thuật toán của hệ thống, bao gồm `RiskEngine` (tổng hợp điểm), `DecisionEngine` (phân luồng hành động) và các luật gian lận con theo mẫu Strategy Pattern.\n"
        "3. Tầng Thực thể Miền Nghiệp vụ (Domain Model Layer): Chứa các đối tượng nghiệp vụ cốt lõi như `Transaction`, `Customer`, `Account`, `PaymentMethod`, `FraudAlert`, `RiskAssessment`, `Device`, `Location`.\n"
        "4. Tầng Lưu trữ & Cấu trúc Dữ liệu Nền tảng (Storage & Handcrafted DSA Layer): Cung cấp giao diện truy xuất dữ liệu `IRepository<T>` độc lập với thiết bị lưu trữ và bộ thư viện DSA tự viết tay (`Vector`, `Deque`, `PriorityQueue`) đạt hiệu năng tối đa O(1)."
    )

    # Insert Diagram 1
    diag1_path = "docs/diagrams/diagram1_architecture.png"
    if os.path.exists(diag1_path):
        p_img = doc.add_paragraph()
        p_img.alignment = WD_ALIGN_PARAGRAPH.CENTER
        p_img.paragraph_format.space_before = Pt(8)
        p_img.paragraph_format.space_after = Pt(4)
        run_img = p_img.add_run()
        run_img.add_picture(diag1_path, width=Inches(6.5))
        
        p_cap = doc.add_paragraph()
        p_cap.alignment = WD_ALIGN_PARAGRAPH.CENTER
        p_cap.paragraph_format.space_after = Pt(12)
        r_cap = p_cap.add_run("Hình 1.1: Sơ đồ kiến trúc phân tầng và mối liên hệ giữa các tập tin trong hệ thống EPFD-RAS")
        r_cap.font.name = "Arial"
        r_cap.font.size = Pt(9)
        r_cap.font.italic = True
        r_cap.font.color.rgb = RGBColor(0x64, 0x74, 0x8B)

    add_callout(doc, 
        "Trong các hệ thống thanh toán điện tử thời gian thực, việc tách bạch giữa Tầng Động Cơ (Engine) "
        "và Tầng Thực Thể (Domain) là nguyên tắc sống còn. Các lớp thực thể (như Transaction, Customer) chỉ nắm giữ "
        "dữ liệu và các phương thức kiểm tra tính hợp lệ cơ bản của chính nó (Encapsulation), trong khi toàn bộ tri thức "
        "về luật gian lận và chính sách quản trị rủi ro được cô lập tại Tầng Engine.",
        "Ý NGHĨA KIẾN TRÚC PHÂN TẦNG"
    )

    # ==============================================================================
    # CHƯƠNG 2
    # ==============================================================================
    h2 = doc.add_heading("CHƯƠNG 2: BẢNG TRA CỨU CHỨC NĂNG TỪNG TẬP TIN VÀ MỐI QUAN HỆ HỆ THỐNG", level=1)
    h2.paragraph_format.space_before = Pt(14)
    h2.paragraph_format.space_after = Pt(6)

    doc.add_paragraph(
        "Dưới đây là bảng tổng hợp chi tiết chức năng, trách nhiệm duy nhất (Single Responsibility) và mối liên hệ "
        "include/phụ thuộc của từng tập tin trong mã nguồn dự án:"
    )

    headers_file = ["Tập tin (Header / Source)", "Tầng Kiến Trúc", "Trách Nhiệm Chính & Chức Năng", "Mối Quan Hệ & Phụ Thuộc"]
    widths_file = [1.8, 1.1, 2.2, 1.4]
    
    file_table_data = [
        ["include/core/common/Types.h\nsources/core/common/Types.cpp", 
         "Common Types", 
         "Khai báo toàn bộ enum class trung tâm: TransactionType, TransactionStatus, RiskLevel, DecisionAction... Cung cấp hàm chuyển đổi toString() và nạp chồng operator<<.",
         "Là nền tảng cốt lõi, được hầu hết các tập tin Domain và Engine #include."],
         
        ["include/core/domain/location.h\nsources/core/domain/location.cpp", 
         "Domain Model", 
         "Value Object biểu diễn tọa độ địa lý (kinh độ, vĩ độ, quốc gia, thành phố). Cung cấp phương thức distanceTo() tính khoảng cách haversine giữa 2 vị trí.",
         "Được nhúng chặt chẽ (Composition) vào Transaction, Customer, Device, Merchant."],
         
        ["include/core/domain/device.h\nsources/core/domain/device.cpp", 
         "Domain Model", 
         "Đại diện cho thiết bị thực hiện thanh toán (IP, vân tay phần cứng, cờ máy ảo emulator, cờ thiết bị root/hacker). Kiểm tra độ an toàn is_Safe().",
         "Sở hữu Location. Được nhúng chặt chẽ (Composition) vào Transaction."],
         
        ["include/core/domain/Account.h\nsources/core/domain/Account.cpp", 
         "Domain Model", 
         "Quản lý số dư tiền tệ (Balance), lịch sử giao dịch của tài khoản ngân hàng. Cung cấp hàm nạp tiền deposit(), rút tiền withdraw(), đóng băng freeze().",
         "Phụ thuộc Types.h. Được quản lý bởi AccountRepo. Không phụ thuộc vào Transaction."],
         
        ["include/core/domain/Customer.h\nsources/core/domain/Customer.cpp", 
         "Domain Model", 
         "Thực thể khách hàng, thông tin định danh KYC, hạng rủi ro cơ sở (CustomerRiskTier, baseline_risk_score), danh sách tài khoản và thiết bị tin cậy.",
         "Sở hữu Location (home_location). Được quản lý bởi CustomerRepo. Là đối số đầu vào của RiskEngine."],
         
        ["include/core/domain/PaymentMethod.h\nsources/core/domain/PaymentMethod.cpp", 
         "Domain Model", 
         "Xử lý phương thức thanh toán thẻ/ví/ngân hàng. Triển khai thuật toán Luhn (validateLuhn) và che số thẻ PAN (maskPan) tuân thủ chuẩn bảo mật thanh toán PCI-DSS.",
         "Cung cấp Factory Methods tĩnh: createBankTransfer(), createEWallet(). Nhúng trong Transaction."],
         
        ["include/core/domain/Transaction.h\nsources/core/domain/transaction.cpp", 
         "Domain Model", 
         "Thực thể trung tâm của luồng xử lý: mã GD, số tiền, loại giao dịch, thời điểm, trạng thái. Quản lý chuyển đổi trạng thái: approve(), reject(), sendToReview().",
         "Hợp thành (Composition) với Location và Device. Là đối tượng thẩm định của toàn bộ các Rules và Engines."],
         
        ["include/core/domain/FraudAlert.h\nsources/core/domain/FraudAlert.cpp", 
         "Domain Model", 
         "Đóng gói cảnh báo vi phạm của một luật cụ thể: rule_id, rule_name, category, điểm đóng góp rủi ro (risk_contribution), mức độ nghiêm trọng (RiskLevel).",
         "Được khởi tạo bởi các lớp luật (IFraudRule) và nạp vào danh sách của RiskAssessment."],
         
        ["include/core/domain/RiskAssessment.h\nsources/core/domain/RiskAssessment.cpp", 
         "Domain Model", 
         "Bản kết quả đánh giá rủi ro toàn diện: điểm luật (rule_score), điểm ML (ml_score), điểm tổng hợp (combined_score), danh sách FraudAlert và lý do vi phạm.",
         "Được khởi tạo và trả về bởi RiskEngine::assessRisk(). Là đầu vào cho DecisionEngine::decide()."],
         
        ["include/engine/rules/IFraudRule.h", 
         "Engine (Rules)", 
         "Giao diện thuần ảo (Pure Virtual Interface) áp dụng Strategy Pattern. Định nghĩa hợp đồng: evaluate(tx, customer, out_alert), getId(), getRiskContribution().",
         "Lớp cha cơ sở của 4 luật cụ thể. Được lưu trữ dưới dạng con trỏ IFraudRule* trong RiskEngine."],
         
        ["include/engine/rules/HighAmountRule.h\n.cpp", 
         "Engine (Rules)", 
         "Luật kiểm tra giao dịch có số tiền vượt ngưỡng cho phép (max_amount) hoặc đột biến gấp nhiều lần mức chi tiêu trung bình của khách hàng.",
         "Kế thừa công khai từ IFraudRule. Hiện thực hàm evaluate(). Phụ thuộc Transaction và Customer."],
         
        ["include/engine/rules/VelocityRule.h\n.cpp", 
         "Engine (Rules)", 
         "Luật phát hiện tấn công dồn dập (Spam quẹt thẻ trong thời gian ngắn) bằng kỹ thuật Cửa sổ trượt (Sliding Window).",
         "Kế thừa IFraudRule. Hợp thành (Composition) với cấu trúc dữ liệu Deque<Timestamp> tự viết tay O(1)."],
         
        ["include/engine/rules/DeviceRiskRule.h\n.cpp", 
         "Engine (Rules)", 
         "Luật thẩm định mức độ nguy hiểm của thiết bị: kích hoạt cảnh báo nếu giao dịch phát sinh từ môi trường giả lập (emulator) hoặc máy đã bị root/jailbreak.",
         "Kế thừa IFraudRule. Truy xuất phương thức kiểm tra an toàn từ đối tượng Device trong Transaction."],
         
        ["include/engine/rules/BlacklistRule.h\n.cpp", 
         "Engine (Rules)", 
         "Luật tra cứu danh sách đen: kiểm tra xem địa chỉ IP hoặc số tài khoản đích có nằm trong cơ sở dữ liệu gian lận đã biết hay không với tốc độ O(1).",
         "Kế thừa IFraudRule. Sử dụng std::unordered_map để tra cứu danh sách đen tức thì."],
         
        ["include/engine/NativeMLPredictor.h\n.cpp", 
         "Engine (ML)", 
         "Động cơ suy luận học máy nhẹ (C++ Native Inference). Nạp trọng số từ file JSON (xuất từ Python). Tính xác suất gian lận bằng hàm Sigmoid: 1 / (1 + e^-z).",
         "Độc lập hoàn toàn với thư viện bên ngoài. Sử dụng Vector<double> tự viết. Cung cấp điểm ML cho RiskEngine."],
         
        ["include/engine/RiskEngine.h\n.cpp", 
         "Engine Core", 
         "Nhạc trưởng tổng hợp rủi ro: quản lý mảng đa hình Vector<IFraudRule*>, duyệt đánh giá từng luật, tích hợp điểm ML theo trọng số cấu hình (RiskPolicy).",
         "Thu nạp (Aggregation) danh sách IFraudRule*. Phụ thuộc Transaction, Customer. Sinh ra RiskAssessment."],
         
        ["include/engine/DecisionEngine.h\n.cpp", 
         "Engine Core", 
         "Bộ não phân luồng hành động: căn cứ trên điểm rủi ro và các ngưỡng DecisionPolicy để đưa ra quyết định: APPROVE, CHALLENGE_3DS, REVIEW, BLOCK.",
         "Phụ thuộc RiskAssessment. Trực tiếp gọi hàm chuyển đổi trạng thái trên thực thể Transaction."],
         
        ["include/storage/IRepository.h", 
         "Storage", 
         "Giao diện lưu trữ khuôn mẫu tổng quát (Generic Template Interface) theo mẫu Repository Pattern: add(), findById(), getAll(), update(), remove().",
         "Là giao diện trừu tượng thuần ảo cho các lớp Repository cụ thể."],
         
        ["include/storage/AccountRepo.h\nCustomerRepo.h", 
         "Storage", 
         "Hiện thực cụ thể IRepository cho Account và Customer. Quản lý việc đọc, ghi, tìm kiếm và lưu trữ bền vững (Persistence) dữ liệu từ tập tin văn bản.",
         "Kế thừa từ IRepository<T>. Thu nạp std::vector<Account> và std::vector<Customer>."],
         
        ["include/DSA/vector.h, Deque.h\npriorityqueue.h, queue.h", 
         "Custom DSA", 
         "Bộ thư viện cấu trúc dữ liệu tự viết tay chuẩn mực: Vector (Mảng động), Deque (Hàng đợi 2 đầu vòng), PriorityQueue (Heap ưu tiên Top-K rủi ro).",
         "Header-only template. Đảm bảo độc lập 100% với STL container trong các module thuật toán trọng yếu."],
         
        ["sources/main.cpp", 
         "Application", 
         "Điểm khởi chạy chương trình (Entry Point), cấu hình toàn bộ hệ thống, khởi tạo các luật và kịch bản kiểm thử mẫu (Showcase Console Demo).",
         "Điều phối toàn bộ các module: khởi tạo Repository, đăng ký Rules vào RiskEngine, thực thi kiểm thử."]
    ]
    
    t_file = doc.add_table(rows=1, cols=4)
    format_table(t_file, widths_file, headers_file, file_table_data)

    doc.add_page_break()

    # ==============================================================================
    # CHƯƠNG 3
    # ==============================================================================
    h3 = doc.add_heading("CHƯƠNG 3: SƠ ĐỒ THIẾT KẾ HƯỚNG ĐỐI TƯỢNG (UML CLASS DIAGRAM)", level=1)
    h3.paragraph_format.space_before = Pt(14)
    h3.paragraph_format.space_after = Pt(6)

    doc.add_paragraph(
        "Sơ đồ lớp UML (Unified Modeling Language) dưới đây mô tả cấu trúc chi tiết của các lớp trong hệ thống EPFD-RAS, "
        "thể hiện các thuộc tính thành viên (attributes), phương thức (methods) và đặc biệt là các mối quan hệ cấu trúc "
        "chuẩn mực trong Lập trình Hướng đối tượng:"
    )

    # Insert Diagram 2
    diag2_path = "docs/diagrams/diagram2_class_diagram.png"
    if os.path.exists(diag2_path):
        p_img2 = doc.add_paragraph()
        p_img2.alignment = WD_ALIGN_PARAGRAPH.CENTER
        p_img2.paragraph_format.space_before = Pt(8)
        p_img2.paragraph_format.space_after = Pt(4)
        run_img2 = p_img2.add_run()
        run_img2.add_picture(diag2_path, width=Inches(6.5))
        
        p_cap2 = doc.add_paragraph()
        p_cap2.alignment = WD_ALIGN_PARAGRAPH.CENTER
        p_cap2.paragraph_format.space_after = Pt(12)
        r_cap2 = p_cap2.add_run("Hình 3.1: Sơ đồ lớp chi tiết thiết kế hướng đối tượng (UML Class Diagram) của hệ thống EPFD-RAS")
        r_cap2.font.name = "Arial"
        r_cap2.font.size = Pt(9)
        r_cap2.font.italic = True
        r_cap2.font.color.rgb = RGBColor(0x64, 0x74, 0x8B)

    doc.add_paragraph(
        "Ý nghĩa màu sắc và ký hiệu trên Sơ đồ lớp UML:\n"
        "• Màu Tím (<<interface>> IFraudRule & Rules): Phân hệ phát hiện gian lận áp dụng mẫu thiết kế Strategy Pattern.\n"
        "• Màu Xanh Mòng Két (Engines): Phân hệ xử lý trung tâm (RiskEngine, DecisionEngine, NativeMLPredictor).\n"
        "• Màu Đỏ Rực (Domain Entities): Các thực thể nghiệp vụ thanh toán cốt lõi.\n"
        "• Mũi tên nét đứt đầu tam giác rỗng (- - |>): Mối quan hệ Hiện thực hóa giao diện (Realization / Implementation).\n"
        "• Hình thoi đặc đen (◆--): Mối quan hệ Hợp thành (Composition) - sở hữu trọn vẹn vòng đời.\n"
        "• Hình thoi rỗng trắng (◇--): Mối quan hệ Thu nạp (Aggregation) - tham chiếu lỏng lẻo qua con trỏ."
    )

    doc.add_page_break()

    # ==============================================================================
    # CHƯƠNG 4
    # ==============================================================================
    h4 = doc.add_heading("CHƯƠNG 4: PHÂN TÍCH CHUYÊN SÂU 4 TÍNH CHẤT HƯỚNG ĐỐI TƯỢNG (OOP)", level=1)
    h4.paragraph_format.space_before = Pt(14)
    h4.paragraph_format.space_after = Pt(6)

    doc.add_paragraph(
        "Tính ưu việt và độ bền vững của mã nguồn C++ trong dự án EPFD-RAS đạt được là nhờ việc áp dụng sâu sắc, "
        "chuẩn mực 4 trụ cột cơ bản của Lập trình Hướng đối tượng. Dưới đây là phân tích chi tiết từng tính chất "
        "gắn liền với bằng chứng trực tiếp trong mã nguồn dự án:"
    )

    # 4.1 Encapsulation
    h4_1 = doc.add_heading("4.1. Tính Đóng Gói (Encapsulation) & Che Giấu Dữ Liệu (Data Hiding)", level=2)
    h4_1.paragraph_format.space_before = Pt(10)
    h4_1.paragraph_format.space_after = Pt(4)

    doc.add_paragraph(
        "Tính đóng gói không đơn thuần là việc đặt từ khóa `private` cho các biến thành viên và viết các hàm getter/setter "
        "thông thường. Trong dự án EPFD-RAS, tính đóng gói được nâng lên mức bảo vệ bất biến dữ liệu (Invariant Protection) "
        "và đóng gói máy trạng thái hữu hạn (Finite State Machine):"
    )
    doc.add_paragraph(
        "1. Bảo vệ tính bất biến của Tài khoản (Account Invariants): "
        "Thuộc tính `balance` (số dư) và `status` (trạng thái) của lớp Account được đặt là `private`. Không có bất kỳ phương thức nào "
        "cho phép bên ngoài gán trực tiếp giá trị số dư (ví dụ `setBalance()`). Mọi biến động số dư bắt buộc phải đi qua nghiệp vụ: "
        "`deposit(amount)` (kiểm tra amount > 0) và `withdraw(amount)` (kiểm tra tài khoản có bị đóng băng hay không và "
        "`hasSufficientBalance(amount)`). Điều này ngăn chặn triệt để lỗi số dư âm hoặc thao túng số dư trái phép.\n\n"
        "2. Đóng gói máy trạng thái giao dịch (Transaction Lifecycle Encapsulation): "
        "Lớp `Transaction` không cho phép gán trạng thái tự do. Thay vào đó, nó cung cấp các hàm chuyển đổi trạng thái mang tính nghiệp vụ: "
        "`approve()`, `reject()`, `sendToReview()`, `challenge()`. Mỗi phương thức này tự động kiểm tra trạng thái hiện tại (chỉ PENDING mới được duyệt/từ chối), "
        "cập nhật mốc thời gian `updated_at` và ngăn chặn việc duyệt lại một giao dịch đã bị chặn.\n\n"
        "3. Đóng gói bảo mật thanh toán chuẩn PCI-DSS (Payment Data Redaction): "
        "Lớp `PaymentMethod` đóng gói hoàn toàn số thẻ thanh toán thực tế. Thông qua phương thức nội bộ `maskPan()`, số thẻ 16 chữ số được che giấu "
        "thành dạng `4111 11** **** 1234`. Mã CVV hoàn toàn bị loại bỏ khỏi bộ nhớ sau khi kiểm tra. Thuật toán Luhn kiểm tra tính hợp lệ của số thẻ "
        "được đóng gói gọn gàng bên trong phương thức tĩnh `validateLuhn()`.\n\n"
        "4. Đóng gói cấu trúc dữ liệu thuật toán phức tạp: "
        "Lớp `VelocityRule` sử dụng cấu trúc dữ liệu `Deque<Timestamp>` để hiện thực thuật toán Cửa sổ trượt (Sliding Time Window). "
        "Bên ngoài chỉ nhìn thấy duy nhất phương thức `evaluate()`, toàn bộ thao tác thêm cuối `push_back()`, lọc bỏ các mốc thời gian quá hạn ở đầu "
        "`pop_front()` và tính toán tần suất đều được che giấu tuyệt đối bên trong lớp."
    )

    add_callout(doc,
        "Nguyên tắc cốt lõi của Encapsulation trong C++ là: 'Hãy yêu cầu đối tượng thực hiện hành động (Tell, Don't Ask), "
        "đừng hỏi dữ liệu của đối tượng để tự thực hiện tính toán bên ngoài'. Việc cung cấp tx.approve() thay vì "
        "tx.setStatus(APPROVED) chính là hiện thân mẫu mực của nguyên lý này.",
        "TRIẾT LÝ THIẾT KẾ ĐÓNG GÓI"
    )

    # 4.2 Inheritance
    h4_2 = doc.add_heading("4.2. Tính Kế Thừa (Inheritance) & Tầm Quan Trọng Của Virtual Destructor", level=2)
    h4_2.paragraph_format.space_before = Pt(10)
    h4_2.paragraph_format.space_after = Pt(4)

    doc.add_paragraph(
        "Hệ thống áp dụng Kế thừa Giao diện (Interface Inheritance) thay vì Kế thừa Hiện thực cồng kềnh (Implementation Inheritance), "
        "giúp tránh hoàn toàn bài toán 'Fragile Base Class' và sự phụ thuộc quá mức vào lớp cha:"
    )
    doc.add_paragraph(
        "• Giao diện `IFraudRule`: Là lớp cơ sở thuần ảo (Abstract Base Class / Interface). Nó định nghĩa bản hợp đồng tối thiểu mà mọi luật gian lận "
        "bắt buộc phải có: `getId()`, `getName()`, `getCategory()`, `getRiskContribution()` và `evaluate()`.\n"
        "• 4 Lớp con kế thừa công khai (`public IFraudRule`): `HighAmountRule`, `VelocityRule`, `DeviceRiskRule`, `BlacklistRule`. Mỗi lớp con kế thừa "
        "toàn bộ định nghĩa kiểu dữ liệu nhưng tự do hiện thực thuật toán nhận diện gian lận chuyên biệt theo nghiệp vụ của mình.\n"
        "• Kế thừa khuôn mẫu `IRepository<T>`: Lớp `AccountRepo` và `CustomerRepo` kế thừa từ giao diện trừu tượng `IRepository<T>`, hiện thực hóa "
        "đầy đủ các phương thức CRUD chuẩn mực."
    )

    doc.add_paragraph(
        "⚠️ Vấn đề Sống Còn: Tại sao bắt buộc phải có Virtual Destructor (`virtual ~IFraudRule() = default;`)?\n"
        "Trong lớp `RiskEngine`, các luật được quản lý dưới dạng mảng con trỏ lớp cơ sở: `Vector<IFraudRule*> rules`. "
        "Khi đối tượng `RiskEngine` bị hủy (Destructor), nó sẽ duyệt qua danh sách và gọi lệnh `delete rules[i]` trên từng con trỏ `IFraudRule*`.\n"
        "Nếu destructor trong `IFraudRule` KHÔNG ĐƯỢC KHAI BÁO LÀ VIRTUAL:\n"
        "1. Trình biên dịch C++ chỉ thực hiện 'Early Binding' và gọi destructor của lớp cha `IFraudRule`.\n"
        "2. Destructor của các lớp con (ví dụ `VelocityRule` sở hữu mảng động `Deque`, hay `BlacklistRule` sở hữu `unordered_map`) SẼ KHÔNG BAO GIỜ ĐƯỢC GỌI!\n"
        "3. Hậu quả: Dẫn đến Hành vi không xác định (Undefined Behavior) và rò rỉ bộ nhớ nghiêm trọng (Memory Leak), làm sập ứng dụng sau một thời gian vận hành.\n"
        "Do đó, việc khai báo `virtual ~IFraudRule() = default;` là bắt buộc để đảm bảo cơ chế 'Dynamic Dispatch' kích hoạt chính xác destructor của lớp con tương ứng."
    )

    # 4.3 Polymorphism
    h4_3 = doc.add_heading("4.3. Tính Đa Hình (Polymorphism): Đa Hình Động vs. Đa Hình Tĩnh", level=2)
    h4_3.paragraph_format.space_before = Pt(10)
    h4_3.paragraph_format.space_after = Pt(4)

    doc.add_paragraph(
        "Dự án kết hợp hài hòa cả hai hình thức Đa hình trong C++ hiện đại, mang lại sự linh hoạt tối đa ở tầng nghiệp vụ "
        "và thông lượng cực cao ở tầng cấu trúc dữ liệu:"
    )

    doc.add_paragraph(
        "1. Đa hình Động tại thời gian chạy (Runtime Polymorphism / Dynamic Dispatch via vtable):\n"
        "• Lớp `RiskEngine` không hề biết trước hệ thống đang có cụ thể những luật nào (nó chỉ biết giao diện `IFraudRule`).\n"
        "• Khi phương thức `RiskEngine::assessRisk()` được thực thi, vòng lặp duyệt qua mảng `Vector<IFraudRule*>`:\n"
        "     for (int i = 0; i < rules.getSize(); ++i) {\n"
        "         FraudAlert alert;\n"
        "         bool is_violated = rules[i]->evaluate(tx, customer, alert);\n"
        "         ...\n"
        "     }\n"
        "• Nhờ có bảng phương thức ảo (vtable) và con trỏ bảng ảo (vptr) được gắn vào mỗi đối tượng, lời gọi `rules[i]->evaluate()` "
        "được điều hướng chính xác đến hàm của `VelocityRule`, `HighAmountRule` hoặc `BlacklistRule` tương ứng tại thời điểm chạy.\n"
        "• Giá trị kiến trúc: Triệt tiêu hoàn toàn các câu lệnh rẽ nhánh cồng kềnh `switch(rule_type)` hoặc chuỗi `if-else` dài hàng chục dòng. "
        "Khi bổ sung thêm 10 luật mới, mã nguồn của `RiskEngine` hoàn toàn không cần phải sửa đổi một dòng nào."
    )

    doc.add_paragraph(
        "2. Đa hình Tĩnh tại thời gian biên dịch (Compile-time Polymorphism via C++ Templates):\n"
        "• Áp dụng trong bộ thư viện cấu trúc dữ liệu tự viết tay: `Vector<T>`, `Deque<T>`, `PriorityQueue<T>` và giao diện `IRepository<T>`.\n"
        "• Cơ chế: Trình biên dịch C++ thực hiện quá trình 'Monomorphization' (Template Instantiation), sinh ra mã máy chuyên biệt cho từng kiểu dữ liệu "
        "(ví dụ `Vector<double>`, `Vector<IFraudRule*>`, `Deque<Timestamp>`).\n"
        "• Lợi thế vượt trội: Không tốn chi phí gián tiếp (Indirection Overhead) của con trỏ vptr/vtable, cho phép trình biên dịch thực hiện tối ưu hóa "
        "nội suy hàm (Inlining) tối đa. Nhờ đó, động cơ đạt thông lượng kỷ lục 165,000+ giao dịch/giây với độ trễ chỉ 6.06 microsecond."
    )

    doc.add_paragraph(
        "3. Nạp chồng hàm và toán tử (Function & Operator Overloading):\n"
        "• Nạp chồng toán tử xuất luồng `operator<<(std::ostream& os, RiskLevel level)` và các enum khác trong `Types.h`, cho phép in trực tiếp "
        "các kiểu dữ liệu miền ra màn hình console hoặc file log một cách tự nhiên: `std::cout << risk_level;`.\n"
        "• Nạp chồng Constructor cho `RiskEngine(custom_policy)`, `Transaction`, `Account` giúp khởi tạo đối tượng linh hoạt với các tham số mặc định."
    )

    # 4.4 Abstraction
    h4_4 = doc.add_heading("4.4. Tính Trừu Tượng (Abstraction) & Bức Tường Lửa Biên Dịch", level=2)
    h4_4.paragraph_format.space_before = Pt(10)
    h4_4.paragraph_format.space_after = Pt(4)

    doc.add_paragraph(
        "Tính trừu tượng thể hiện ở việc mô hình hóa các thực thể thế giới thực thành các khái niệm cốt lõi trong phần mềm, "
        "bỏ qua các chi tiết thừa thãi không phục vụ cho mục tiêu phát hiện gian lận:\n"
        "• Trừu tượng hóa Luật gian lận: Dù mỗi luật có thuật toán hoàn toàn khác nhau (Velocity dùng thời gian, HighAmount so sánh tiền, "
        "Blacklist tra cứu băm), chúng đều được trừu tượng hóa thành một 'hộp đen' nhận vào `(Transaction, Customer)` và trả về `(bool, FraudAlert)`.\n"
        "• Trừu tượng hóa Lưu trữ: Tầng nghiệp vụ không cần biết dữ liệu được lưu trong file text, SQLite hay cơ sở dữ liệu phân tán. Nó chỉ giao tiếp "
        "với trừu tượng `IRepository<T>` thông qua các thao tác `add()`, `findById()`.\n"
        "• Phân chia Header (`.h`) và Implementation (`.cpp`): Header đóng vai trò là hợp đồng công khai (Public Interface), còn toàn bộ mã nguồn "
        "xử lý chi tiết được ẩn kín trong file `.cpp`. Điều này đóng vai trò như một bức tường lửa biên dịch (Compilation Firewall), bảo vệ quyền riêng tư "
        "của thuật toán và giúp lập trình viên kiểm thử độc lập từng module."
    )

    doc.add_page_break()

    # ==============================================================================
    # CHƯƠNG 5
    # ==============================================================================
    h5 = doc.add_heading("CHƯƠNG 5: PHÂN TÍCH CHI TIẾT CÁC MỐI QUAN HỆ GIỮA CÁC LỚP (UML RELATIONSHIPS)", level=1)
    h5.paragraph_format.space_before = Pt(14)
    h5.paragraph_format.space_after = Pt(6)

    doc.add_paragraph(
        "Một trong những tiêu chí quan trọng nhất khi đánh giá đồ án Hướng đối tượng là khả năng phân biệt chính xác "
        "và sử dụng đúng ngữ cảnh các mối quan hệ giữa các lớp. Dưới đây là phân tích chi tiết 5 mối quan hệ UML cốt lõi "
        "được cài đặt trong dự án EPFD-RAS:"
    )

    headers_rel = ["Mối Quan Hệ UML", "Ký Hiệu UML", "Bản Chất Trong Mã Nguồn C++", "Ví Dụ Cụ Thể Trong Dự Án EPFD-RAS"]
    widths_rel = [1.5, 1.1, 2.3, 1.6]
    
    data_rel = [
        ["1. Kế Thừa (Generalization / Inheritance)",
         "───▷ (Mũi tên rỗng liền nét)",
         "Mối quan hệ 'IS-A'. Lớp con kế thừa giao diện hoặc thuộc tính của lớp cha. Dùng cú pháp `class Con : public Cha`.",
         "HighAmountRule, VelocityRule, DeviceRiskRule kế thừa từ IFraudRule."],
         
        ["2. Hiện Thực Hóa (Realization / Implementation)",
         "- - - ▷ (Mũi tên rỗng đứt nét)",
         "Mối quan hệ lớp cụ thể hiện thực hóa các phương thức thuần ảo (`= 0`) của một Interface trừu tượng.",
         "AccountRepo và CustomerRepo hiện thực hóa giao diện mẫu IRepository<T>."],
         
        ["3. Hợp Thành (Composition)",
         "◆─── (Hình thoi đặc đen)",
         "Mối quan hệ 'Part-of / Whole' sở hữu mạnh mẽ. Vòng đời của bộ phận gắn liền tuyệt đối với toàn thể (Cùng sinh cùng tử). Thể hiện bằng biến thành viên đối tượng trực tiếp (Value Member).",
         "• Transaction sở hữu Location và Device.\n• Customer sở hữu home_location.\n• VelocityRule sở hữu Deque<Timestamp> sliding_window."],
         
        ["4. Thu Nạp (Aggregation)",
         "◇─── (Hình thoi rỗng trắng)",
         "Mối quan hệ 'Has-A' sở hữu lỏng lẻo. Toàn thể chứa tham chiếu hoặc con trỏ đến bộ phận, nhưng vòng đời độc lập (Bộ phận vẫn tồn tại khi toàn thể bị hủy). Thể hiện bằng con trỏ hoặc tập hợp tham chiếu.",
         "• RiskEngine thu nạp Vector<IFraudRule*> (Các rules được khởi tạo ngoài và inject vào).\n• RiskAssessment thu nạp danh sách std::vector<FraudAlert>."],
         
        ["5. Phụ Thuộc (Dependency)",
         "- - - > (Mũi tên hở đứt nét)",
         "Mối quan hệ 'Uses-A' tạm thời. Lớp này sử dụng lớp kia qua tham số của hàm, biến cục bộ hoặc kiểu trả về, không lưu trữ làm biến thành viên.",
         "• RiskEngine phụ thuộc Transaction và Customer qua tham số hàm assessRisk().\n• DecisionEngine phụ thuộc RiskAssessment và cập nhật trạng thái Transaction."]
    ]

    t_rel = doc.add_table(rows=1, cols=4)
    format_table(t_rel, widths_rel, headers_rel, data_rel)

    doc.add_paragraph().paragraph_format.space_before = Pt(8)
    doc.add_paragraph(
        "🔍 So sánh sâu sắc giữa Hợp thành (Composition) và Thu nạp (Aggregation) trong dự án:\n"
        "• Trong `Transaction`: `Location location` và `Device device` được khai báo trực tiếp dưới dạng biến thành viên trị (Value Object). "
        "Khi đối tượng `Transaction` được sinh ra, `location` và `device` được cấp phát cùng lúc. Khi `Transaction` bị hủy, hai đối tượng này "
        "cũng tự động bị thu hồi bộ nhớ theo. Đây là mối quan hệ HỢP THÀNH (Composition).\n"
        "• Trong `RiskEngine`: Thuộc tính `rules` lưu trữ các con trỏ `IFraudRule*`. Các đối tượng con (`new HighAmountRule()`) được tạo độc lập "
        "từ bên ngoài và truyền vào thông qua phương thức `registerRule(IFraudRule* rule)`. Nếu `RiskEngine` không tự động `delete`, các đối tượng rule "
        "vẫn có thể tồn tại độc lập trong bộ nhớ. Đây là mối quan hệ THU NẠP (Aggregation)."
    )

    doc.add_page_break()

    # ==============================================================================
    # CHƯƠNG 6
    # ==============================================================================
    h6 = doc.add_heading("CHƯƠNG 6: CÁC MẪU THIẾT KẾ (DESIGN PATTERNS) & NGUYÊN LÝ SOLID", level=1)
    h6.paragraph_format.space_before = Pt(14)
    h6.paragraph_format.space_after = Pt(6)

    doc.add_paragraph(
        "Hệ thống EPFD-RAS áp dụng các Mẫu thiết kế phần mềm kinh điển của nhóm Gang of Four (GoF) kết hợp nhuần nhuyễn "
        "với 5 nguyên lý thiết kế hướng đối tượng SOLID:"
    )

    h6_1 = doc.add_heading("6.1. Các Mẫu Thiết Kế Phần Mềm (Design Patterns)", level=2)
    h6_1.paragraph_format.space_before = Pt(8)
    h6_1.paragraph_format.space_after = Pt(4)

    doc.add_paragraph(
        "1. Strategy Pattern (Mẫu Chiến Lược - Trọng tâm đồ án):\n"
        "• Bối cảnh: Các thuật toán phát hiện gian lận rất đa dạng và thường xuyên thay đổi theo sự phát triển của tội phạm công nghệ cao.\n"
        "• Cài đặt:\n"
        "  - Strategy Interface: `IFraudRule` định nghĩa phương thức `evaluate()`.\n"
        "  - Concrete Strategies: `HighAmountRule`, `VelocityRule`, `DeviceRiskRule`, `BlacklistRule` cài đặt các thuật toán khác nhau.\n"
        "  - Context: `RiskEngine` nắm giữ danh sách các chiến lược và ủy quyền việc kiểm tra cho từng chiến lược tương ứng.\n"
        "• Lợi ích: Cho phép hoán đổi, thêm bớt các luật gian lận linh hoạt lúc chạy chương trình mà không ảnh hưởng đến bộ khung của engine."
    )
    doc.add_paragraph(
        "2. Repository Pattern (Mẫu Lưu Trữ):\n"
        "• Cài đặt: Lớp trừu tượng `IRepository<T>` định nghĩa các thao tác CRUD. Lớp cụ thể `AccountRepo` và `CustomerRepo` "
        "chịu trách nhiệm đọc ghi dữ liệu từ tập tin văn bản (`accounts.txt`, `customers.txt`).\n"
        "• Lợi ích: Giúp tách biệt hoàn toàn logic nghiệp vụ thanh toán khỏi cơ chế lưu trữ vật lý. Trong tương lai, nếu hệ thống chuyển sang "
        "lưu trữ trên MySQL, PostgreSQL hay Redis, toàn bộ Tầng Domain và Tầng Engine giữ nguyên 100% không cần viết lại."
    )
    doc.add_paragraph(
        "3. Static Factory Method Pattern (Mẫu Nhà Máy Tĩnh):\n"
        "• Cài đặt: Trong lớp `PaymentMethod`, các phương thức khởi tạo phức tạp được che giấu phía sau các hàm tĩnh: "
        "`PaymentMethod::createBankTransfer(...)` và `PaymentMethod::createEWallet(...)`.\n"
        "• Lợi ích: Đảm bảo đối tượng được khởi tạo với đầy đủ các thuộc tính hợp lệ theo từng loại phương thức, tránh việc gọi constructor "
        "với danh sách tham số quá dài dễ nhầm lẫn."
    )

    h6_2 = doc.add_heading("6.2. Ứng Dụng 5 Nguyên Lý Thiết Kế Hướng Đối Tượng SOLID", level=2)
    h6_2.paragraph_format.space_before = Pt(8)
    h6_2.paragraph_format.space_after = Pt(4)

    doc.add_paragraph(
        "• S — Single Responsibility Principle (Nguyên lý Đơn Trách Nhiệm):\n"
        "Mỗi lớp chỉ đảm nhận một lý do duy nhất để thay đổi. `Account` chỉ quản lý số dư; `Transaction` chỉ lưu trữ thông tin giao dịch; "
        "`VelocityRule` chỉ tập trung kiểm tra tần suất quẹt thẻ dồn dập; `NativeMLPredictor` chỉ làm nhiệm vụ tính xác suất Sigmoid; "
        "`AccountRepo` chỉ lo đọc ghi tập tin."
    )
    doc.add_paragraph(
        "• O — Open/Closed Principle (Nguyên lý Đóng/Mở - Quan trọng nhất):\n"
        "Hệ thống mở cho việc mở rộng (Open for Extension) nhưng đóng cho việc sửa đổi (Closed for Modification). "
        "Khi muốn bổ sung một luật mới (ví dụ `CryptoScamRule`), ta chỉ cần tạo một file class mới kế thừa `IFraudRule` và đăng ký "
        "`risk_engine.registerRule(new CryptoScamRule())`. Lớp `RiskEngine` không cần phải chỉnh sửa hay biên dịch lại."
    )
    doc.add_paragraph(
        "• L — Liskov Substitution Principle (Nguyên lý Thay thế Liskov):\n"
        "Mọi lớp con của `IFraudRule` (như `DeviceRiskRule`, `BlacklistRule`) đều có thể thay thế hoàn hảo cho con trỏ lớp cha `IFraudRule*` "
        "trong `RiskEngine` mà không làm thay đổi tính đúng đắn của chương trình. Không có bất kỳ luật con nào ném ra biệt lệ bất thường "
        "hoặc vi phạm hợp đồng mà `IFraudRule` đã cam kết."
    )
    doc.add_paragraph(
        "• I — Interface Segregation Principle (Nguyên lý Phân Tách Giao Diện):\n"
        "Giao diện `IFraudRule` và `IRepository<T>` được thiết kế cực kỳ tinh gọn, chỉ chứa đúng những phương thức cần thiết nhất. "
        "Không bắt buộc các lớp con phải hiện thực những phương thức 'thừa thãi' mà chúng không sử dụng."
    )
    doc.add_paragraph(
        "• D — Dependency Inversion Principle (Nguyên lý Đảo Ngược Phụ Thuộc):\n"
        "Các mô-đun cấp cao (`RiskEngine`) không phụ thuộc trực tiếp vào các mô-đun cấp thấp cụ thể (`HighAmountRule`, `VelocityRule`). "
        "Cả hai đều phụ thuộc vào lớp trừu tượng `IFraudRule`. Tương tự, tầng dịch vụ phụ thuộc vào giao diện `IRepository<Account>` chứ không phụ thuộc "
        "trực tiếp vào file vật lý `AccountRepo.cpp`."
    )

    doc.add_page_break()

    # ==============================================================================
    # CHƯƠNG 7
    # ==============================================================================
    h7 = doc.add_heading("CHƯƠNG 7: LUỒNG XỬ LÝ DỮ LIỆU THỜI GIAN THỰC & TƯƠNG TÁC ĐỐI TƯỢNG", level=1)
    h7.paragraph_format.space_before = Pt(14)
    h7.paragraph_format.space_after = Pt(6)

    doc.add_paragraph(
        "Sơ đồ dưới đây mô tả quy trình 6 bước khép kín khi một giao dịch thanh toán được tiếp nhận và xử lý "
        "xuyên suốt qua các đối tượng trong hệ thống EPFD-RAS:"
    )

    # Insert Diagram 3
    diag3_path = "docs/diagrams/diagram3_sequence_flow.png"
    if os.path.exists(diag3_path):
        p_img3 = doc.add_paragraph()
        p_img3.alignment = WD_ALIGN_PARAGRAPH.CENTER
        p_img3.paragraph_format.space_before = Pt(8)
        p_img3.paragraph_format.space_after = Pt(4)
        run_img3 = p_img3.add_run()
        run_img3.add_picture(diag3_path, width=Inches(6.5))
        
        p_cap3 = doc.add_paragraph()
        p_cap3.alignment = WD_ALIGN_PARAGRAPH.CENTER
        p_cap3.paragraph_format.space_after = Pt(12)
        r_cap3 = p_cap3.add_run("Hình 7.1: Sơ đồ luồng xử lý dữ liệu và tương tác giữa các đối tượng qua 6 bước")
        r_cap3.font.name = "Arial"
        r_cap3.font.size = Pt(9)
        r_cap3.font.italic = True
        r_cap3.font.color.rgb = RGBColor(0x64, 0x74, 0x8B)

    doc.add_paragraph(
        "Chi tiết quy trình thực thi 6 bước:\n"
        "• Bước 1 (Tiếp nhận & Chuẩn hóa): Khi sự kiện thanh toán xuất hiện, hệ thống tạo đối tượng `Transaction`, gắn kết đối tượng "
        "`Device` và `Location`. Số thẻ được kiểm tra hợp lệ bằng thuật toán Luhn và che giấu chuẩn PCI-DSS qua `PaymentMethod`.\n"
        "• Bước 2 (Đánh giá Đa Luật Tất Định): `RiskEngine` duyệt qua mảng `Vector<IFraudRule*>`. `VelocityRule` sử dụng cấu trúc `Deque` để đếm "
        "số giao dịch trong 60 giây gần nhất; `BlacklistRule` kiểm tra IP/Account trong bảng băm O(1). Nếu phát hiện vi phạm, một đối tượng "
        "`FraudAlert` được khởi tạo và ghi nhận điểm rủi ro.\n"
        "• Bước 3 (Dự đoán Học Máy Native): Trích xuất vector đặc trưng 4 chiều (số tiền, vận tốc, khoảng cách, rủi ro thiết bị). Lớp `NativeMLPredictor` "
        "tính toán hàm kích hoạt Sigmoid với các trọng số nạp từ `model_weights.json`, trả về xác suất gian lận P(fraud) và quy đổi sang điểm ML (0 - 100).\n"
        "• Bước 4 (Tổng hợp Rủi ro Đa Nhân Tố): `RiskEngine` áp dụng chính sách `RiskPolicy` (ví dụ: 50% điểm luật + 40% điểm ML + 10% rủi ro KYC). "
        "Điểm rủi ro tổng hợp được phân tầng thành `RiskLevel` (VERY_LOW, LOW, MEDIUM, HIGH, CRITICAL) và đóng gói vào `RiskAssessment`.\n"
        "• Bước 5 (Phân Luồng Quyết Định Hành Động): `DecisionEngine` so khớp điểm tổng hợp với `DecisionPolicy`: điểm < 40 duyệt ngay (APPROVE), "
        "điểm 40 - 60 yêu cầu xác thực OTP 3D-Secure (CHALLENGE_3DS), điểm 60 - 80 gửi điều tra viên rà soát (REVIEW), điểm >= 80 lập tức từ chối (BLOCK).\n"
        "• Bước 6 (Cập nhật Trạng thái & Lưu Trữ Bền Vững): Nếu giao dịch được duyệt, phương thức `account.withdraw(amount)` được kích hoạt an toàn. "
        "Nếu giao dịch bị chặn, tiền của khách hàng được giữ nguyên vẹn. Trạng thái giao dịch được lưu vết bất biến thông qua `AccountRepo`."
    )

    doc.add_page_break()

    # ==============================================================================
    # CHƯƠNG 8
    # ==============================================================================
    h8 = doc.add_heading("CHƯƠNG 8: BỘ CÂU HỎI & HƯỚNG DẪN TRẢ LỜI BẢO VỆ ĐỒ ÁN PBL2", level=1)
    h8.paragraph_format.space_before = Pt(14)
    h8.paragraph_format.space_after = Pt(6)

    doc.add_paragraph(
        "Nhằm giúp hai bạn sinh viên hoàn toàn tự tin và làm chủ mã nguồn trước hội đồng chấm thi của các thầy cô, "
        "dưới đây là danh sách 6 câu hỏi kinh điển về Lập trình Hướng đối tượng và Cấu trúc dữ liệu kèm câu trả lời chuẩn xác nhất:"
    )

    qa_list = [
        ("Câu 1: Tại sao trong lớp IFraudRule và IRepository bắt buộc phải có Virtual Destructor?",
         "Trả lời: Trong RiskEngine, chúng em quản lý các luật thông qua con trỏ lớp cơ sở `IFraudRule*`. Khi RiskEngine bị hủy, "
         "nó gọi lệnh `delete` trên con trỏ này. Nếu destructor không phải là virtual, trình biên dịch chỉ gọi destructor của lớp cha `IFraudRule` "
         "mà bỏ qua destructor của lớp con (`VelocityRule`, `BlacklistRule`). Điều này gây rò rỉ bộ nhớ nghiêm trọng (Memory Leak) đối với các cấu trúc "
         "dữ liệu mà lớp con sở hữu như `Deque` hay `unordered_map`. Khai báo `virtual ~IFraudRule() = default;` đảm bảo cơ chế bảng ảo (vtable) "
         "sẽ gọi đúng destructor của lớp con thực tế."),
         
        ("Câu 2: Hệ thống của hai bạn áp dụng mẫu thiết kế (Design Pattern) nào? Tại sao lại chọn mẫu đó?",
         "Trả lời: Dự án áp dụng mẫu Strategy Pattern cho hệ thống thẩm định luật gian lận. Lớp `RiskEngine` đóng vai trò là Context, "
         "`IFraudRule` là Strategy Interface, và các lớp `VelocityRule`, `HighAmountRule`, `DeviceRiskRule`, `BlacklistRule` là các Concrete Strategies. "
         "Chúng em chọn mẫu này vì nó thỏa mãn nguyên lý Open/Closed Principle (OCP): khi cần bổ sung luật mới, chúng em chỉ cần viết thêm một class mới "
         "mà hoàn toàn không phải chỉnh sửa bất kỳ dòng code nào trong `RiskEngine`."),
         
        ("Câu 3: Phân biệt sự khác nhau giữa Đa hình Động và Đa hình Tĩnh trong dự án của bạn?",
         "Trả lời: Đa hình Động (Runtime Polymorphism) được chúng em dùng ở tầng Engine thông qua con trỏ `IFraudRule*` và hàm ảo `evaluate()`. "
         "Hệ thống quyết định gọi luật nào lúc chương trình đang chạy thông qua vtable, giúp code linh hoạt tối đa. "
         "Đa hình Tĩnh (Compile-time Polymorphism) được dùng ở tầng Cấu trúc dữ liệu (DSA) thông qua C++ Template (`Vector<T>`, `Deque<T>`, `PriorityQueue<T>`). "
         "Trình biên dịch sinh mã trực tiếp lúc build, không tốn chi phí tra cứu bảng ảo (Zero Overhead), giúp thuật toán đạt tốc độ xử lý kỷ lục 165,000+ giao dịch/giây."),
         
        ("Câu 4: Mối quan hệ giữa Transaction với Location khác gì mối quan hệ giữa RiskEngine với IFraudRule?",
         "Trả lời: Mối quan hệ giữa `Transaction` và `Location` là quan hệ HỢP THÀNH (Composition). `Location` là biến thành viên trực tiếp của `Transaction`. "
         "Khi `Transaction` được tạo, `Location` được tạo; khi `Transaction` hủy, `Location` hủy theo (cùng sinh cùng tử). "
         "Ngược lại, mối quan hệ giữa `RiskEngine` và `IFraudRule` là quan hệ THU NẠP (Aggregation). `RiskEngine` chỉ lưu danh sách con trỏ `IFraudRule*`. "
         "Các đối tượng luật được khởi tạo độc lập bên ngoài và truyền vào qua `registerRule()`, vòng đời của luật không bị phụ thuộc hoàn toàn vào Engine."),
         
        ("Câu 5: Cấu trúc dữ liệu Deque tự viết tay được áp dụng vào đâu và mang lại lợi ích gì?",
         "Trả lời: Cấu trúc dữ liệu `Deque<Timestamp>` (Hàng đợi hai đầu) được ứng dụng trong lớp `VelocityRule` để hiện thực thuật toán Cửa sổ trượt "
         "(Sliding Time Window). Khi có giao dịch mới, thời gian được đẩy vào cuối Deque `push_back()` tốn O(1). Sau đó, vòng lặp xóa bỏ các giao dịch cũ "
         "quá 60 giây ở đầu Deque `pop_front()` cũng chỉ tốn O(1). Nhờ đó, việc kiểm tra tần suất spam thẻ luôn duy trì độ phức tạp O(1) thay vì phải "
         "quét lại toàn bộ lịch sử tốn O(N)."),
         
        ("Câu 6: Làm thế nào để đảm bảo tính an toàn dữ liệu và tuân thủ chuẩn PCI-DSS trong C++?",
         "Trả lời: Chúng em áp dụng Tính đóng gói (Encapsulation) trong lớp `PaymentMethod`. Thuộc tính số thẻ thực tế được đặt ở chế độ `private`. "
         "Phương thức `maskPan()` tự động chuyển đổi số thẻ thành dạng che giấu `4111 11** **** 1234` trước khi hiển thị ra console hoặc ghi log. "
         "Mã CVV hoàn toàn không được lưu trữ trong thuộc tính lớp. Các thao tác trừ tiền trên `Account` đều có điều kiện kiểm tra hợp lệ nghiêm ngặt, "
         "không có hàm `setBalance()` tùy tiện.")
    ]

    for q, a in qa_list:
        p_q = doc.add_paragraph()
        p_q.paragraph_format.space_before = Pt(8)
        p_q.paragraph_format.space_after = Pt(2)
        r_q = p_q.add_run(f"🔸 {q}")
        r_q.font.bold = True
        r_q.font.size = Pt(10.5)
        r_q.font.color.rgb = RGBColor(0x1E, 0x3A, 0x8A)
        
        p_a = doc.add_paragraph()
        p_a.paragraph_format.space_after = Pt(8)
        p_a.paragraph_format.left_indent = Inches(0.2)
        r_a = p_a.add_run(a)
        r_a.font.size = Pt(10)
        r_a.font.color.rgb = RGBColor(0x33, 0x41, 0x55)

    # Save Document
    output_filename = "Bao_Cao_Thiet_Ke_Huong_Doi_Tuong_EPFD_RAS.docx"
    doc.save(output_filename)
    print(f"Document successfully created: {output_filename}")

if __name__ == "__main__":
    build_document()
