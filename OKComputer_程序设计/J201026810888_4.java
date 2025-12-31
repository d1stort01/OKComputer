import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Java程序设计综合实验
 * 主类：文本编辑器 + 通讯录管理系统
 */
public class J201026810888_4 extends JFrame {
    private JTextArea textArea;
    private JFileChooser fileChooser;
    private File currentFile;
    private boolean isModified = false;
    
    // 字体设置
    private String currentFontName = "宋体";
    private int currentFontSize = 16;
    private Color currentFontColor = Color.BLACK;
    private Color currentBgColor = Color.WHITE;
    
    // 配置文件
    private Properties config;
    private static final String CONFIG_FILE = "editor.config";
    
    // 通讯录相关
    private AddressBook addressBook;
    private static final String ADDRESS_BOOK_FILE = "addressbook.dat";
    
    public J201026810888_4() {
        // 设置窗口标题
        super("201026810888-张三丰-Java程序设计综合实验");
        
        // 初始化组件
        initComponents();
        
        // 加载配置
        loadConfig();
        
        // 初始化通讯录
        addressBook = new AddressBook();
        
        // 设置窗口大小和位置
        setSize(800, 600);
        centerWindow(this);
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        
        // 添加窗口关闭事件
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                exitSystem();
            }
        });
    }
    
    /**
     * 初始化组件
     */
    private void initComponents() {
        // 创建文本区域
        textArea = new JTextArea();
        textArea.setFont(new Font(currentFontName, Font.PLAIN, currentFontSize));
        textArea.setForeground(currentFontColor);
        textArea.setBackground(currentBgColor);
        textArea.getDocument().addDocumentListener(new javax.swing.event.DocumentListener() {
            public void insertUpdate(javax.swing.event.DocumentEvent e) {
                isModified = true;
            }
            public void removeUpdate(javax.swing.event.DocumentEvent e) {
                isModified = true;
            }
            public void changedUpdate(javax.swing.event.DocumentEvent e) {
                isModified = true;
            }
        });
        
        // 添加滚动条
        JScrollPane scrollPane = new JScrollPane(textArea);
        add(scrollPane, BorderLayout.CENTER);
        
        // 创建菜单栏
        createMenuBar();
        
        // 创建文件选择器
        fileChooser = new JFileChooser();
        fileChooser.setFileFilter(new FileNameExtensionFilter("文本文件", "txt", "java", "c", "cpp"));
    }
    
    /**
     * 创建菜单栏
     */
    private void createMenuBar() {
        JMenuBar menuBar = new JMenuBar();
        
        // 文件菜单
        JMenu fileMenu = new JMenu("文件");
        fileMenu.setMnemonic('F');
        
        JMenuItem newItem = new JMenuItem("新建");
        newItem.setMnemonic('N');
        newItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, KeyEvent.CTRL_MASK));
        newItem.addActionListener(e -> newFile());
        
        JMenuItem openItem = new JMenuItem("打开");
        openItem.setMnemonic('O');
        openItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, KeyEvent.CTRL_MASK));
        openItem.addActionListener(e -> openFile());
        
        JMenuItem saveItem = new JMenuItem("保存");
        saveItem.setMnemonic('S');
        saveItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, KeyEvent.CTRL_MASK));
        saveItem.addActionListener(e -> saveFile());
        
        JMenuItem fontItem = new JMenuItem("字体");
        fontItem.addActionListener(e -> setFont());
        
        JMenuItem bgColorItem = new JMenuItem("背景颜色");
        bgColorItem.addActionListener(e -> setBackgroundColor());
        
        JMenuItem exitItem = new JMenuItem("退出");
        exitItem.setMnemonic('E');
        exitItem.addActionListener(e -> exitSystem());
        
        fileMenu.add(newItem);
        fileMenu.add(openItem);
        fileMenu.add(saveItem);
        fileMenu.addSeparator();
        fileMenu.add(fontItem);
        fileMenu.add(bgColorItem);
        fileMenu.addSeparator();
        fileMenu.add(exitItem);
        
        // Java上机题目菜单
        JMenu javaMenu = new JMenu("Java上机题目");
        
        JMenuItem palindromeItem = new JMenuItem("回文数");
        palindromeItem.addActionListener(e -> checkPalindrome());
        
        JMenuItem translateItem = new JMenuItem("数字与英文互译");
        translateItem.addActionListener(e -> translateNumber());
        
        JMenuItem statisticsItem = new JMenuItem("统计英文数据");
        statisticsItem.addActionListener(e -> statisticsWords());
        
        JMenuItem phoneItem = new JMenuItem("手机号码合法性判断");
        phoneItem.addActionListener(e -> checkPhoneNumber());
        
        JMenuItem sumItem = new JMenuItem("文本文件求和");
        sumItem.addActionListener(e -> sumFile());
        
        javaMenu.add(palindromeItem);
        javaMenu.add(translateItem);
        javaMenu.add(statisticsItem);
        javaMenu.add(phoneItem);
        javaMenu.add(sumItem);
        
        // 通讯录菜单
        JMenu addressMenu = new JMenu("通讯录");
        
        JMenuItem maintainItem = new JMenuItem("通讯录维护");
        maintainItem.addActionListener(e -> maintainAddressBook());
        
        JMenuItem settingItem = new JMenuItem("通讯录存储文件设置");
        settingItem.addActionListener(e -> setAddressBookFile());
        
        addressMenu.add(maintainItem);
        addressMenu.add(settingItem);
        
        menuBar.add(fileMenu);
        menuBar.add(javaMenu);
        menuBar.add(addressMenu);
        
        setJMenuBar(menuBar);
    }
    
    /**
     * 窗口居中
     */
    public static void centerWindow(Window f) {
        Toolkit tk = f.getToolkit();
        Dimension dm = tk.getScreenSize();
        f.setLocation((int)(dm.getWidth() - f.getWidth()) / 2, 
                     (int)(dm.getHeight() - f.getHeight()) / 2);
    }
    
    /**
     * 新建文件
     */
    private void newFile() {
        if (isModified) {
            int result = showSaveConfirmDialog();
            if (result == JOptionPane.YES_OPTION) {
                if (!saveFile()) return;
            } else if (result == JOptionPane.CANCEL_OPTION) {
                return;
            }
        }
        textArea.setText("");
        currentFile = null;
        isModified = false;
        setTitle("201026810888-张三丰-Java程序设计综合实验");
    }
    
    /**
     * 打开文件
     */
    private void openFile() {
        if (isModified) {
            int result = showSaveConfirmDialog();
            if (result == JOptionPane.YES_OPTION) {
                if (!saveFile()) return;
            } else if (result == JOptionPane.CANCEL_OPTION) {
                return;
            }
        }
        
        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                BufferedReader reader = new BufferedReader(new FileReader(file));
                StringBuilder content = new StringBuilder();
                String line;
                while ((line = reader.readLine()) != null) {
                    content.append(line).append("\n");
                }
                reader.close();
                
                textArea.setText(content.toString());
                currentFile = file;
                isModified = false;
                setTitle("201026810888-张三丰-Java程序设计综合实验-" + file.getAbsolutePath());
            } catch (IOException e) {
                JOptionPane.showMessageDialog(this, "文件读取失败: " + e.getMessage(), 
                                            "错误", JOptionPane.ERROR_MESSAGE);
            }
        }
    }
    
    /**
     * 保存文件
     */
    private boolean saveFile() {
        if (currentFile == null) {
            return saveAsFile();
        } else {
            try {
                BufferedWriter writer = new BufferedWriter(new FileWriter(currentFile));
                writer.write(textArea.getText());
                writer.close();
                isModified = false;
                return true;
            } catch (IOException e) {
                JOptionPane.showMessageDialog(this, "文件保存失败: " + e.getMessage(), 
                                            "错误", JOptionPane.ERROR_MESSAGE);
                return false;
            }
        }
    }
    
    /**
     * 另存为文件
     */
    private boolean saveAsFile() {
        if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            if (!file.getName().contains(".")) {
                file = new File(file.getAbsolutePath() + ".txt");
            }
            try {
                BufferedWriter writer = new BufferedWriter(new FileWriter(file));
                writer.write(textArea.getText());
                writer.close();
                currentFile = file;
                isModified = false;
                setTitle("201026810888-张三丰-Java程序设计综合实验-" + file.getAbsolutePath());
                return true;
            } catch (IOException e) {
                JOptionPane.showMessageDialog(this, "文件保存失败: " + e.getMessage(), 
                                            "错误", JOptionPane.ERROR_MESSAGE);
                return false;
            }
        }
        return false;
    }
    
    /**
     * 显示保存确认对话框
     */
    private int showSaveConfirmDialog() {
        return JOptionPane.showConfirmDialog(this, "当前文档已修改，是否保存？", 
                                           "提示", JOptionPane.YES_NO_CANCEL_OPTION);
    }
    
    /**
     * 设置字体
     */
    private void setFont() {
        FontDialog dialog = new FontDialog(this);
        dialog.setVisible(true);
        if (dialog.isOk()) {
            currentFontName = dialog.getSelectedFontName();
            currentFontSize = dialog.getSelectedFontSize();
            currentFontColor = dialog.getSelectedColor();
            
            textArea.setFont(new Font(currentFontName, Font.PLAIN, currentFontSize));
            textArea.setForeground(currentFontColor);
            
            saveConfig();
        }
    }
    
    /**
     * 设置背景颜色
     */
    private void setBackgroundColor() {
        Color color = JColorChooser.showDialog(this, "选择背景颜色", currentBgColor);
        if (color != null) {
            currentBgColor = color;
            textArea.setBackground(currentBgColor);
            saveConfig();
        }
    }
    
    /**
     * 退出系统
     */
    private void exitSystem() {
        if (isModified) {
            int result = showSaveConfirmDialog();
            if (result == JOptionPane.YES_OPTION) {
                if (!saveFile()) return;
            } else if (result == JOptionPane.CANCEL_OPTION) {
                return;
            }
        }
        
        int result = JOptionPane.showConfirmDialog(this, "是否要退出系统？", 
                                                 "退出确认", JOptionPane.YES_NO_OPTION);
        if (result == JOptionPane.YES_OPTION) {
            System.exit(0);
        }
    }
    
    /**
     * 检查回文数
     */
    private void checkPalindrome() {
        JDialog dialog = new JDialog(this, "回文数判断", true);
        dialog.setSize(400, 200);
        centerWindow(dialog);
        
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        JPanel inputPanel = new JPanel(new FlowLayout());
        inputPanel.add(new JLabel("请输入1-99999之间的整数："));
        JTextField textField = new JTextField(15);
        inputPanel.add(textField);
        
        JPanel buttonPanel = new JPanel(new FlowLayout());
        JButton checkButton = new JButton("判断是否为回文数");
        JButton cancelButton = new JButton("取消");
        
        buttonPanel.add(checkButton);
        buttonPanel.add(cancelButton);
        
        panel.add(inputPanel, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        checkButton.addActionListener(e -> {
            String input = textField.getText().trim();
            if (input.isEmpty()) {
                JOptionPane.showMessageDialog(dialog, "请输入数字！", "提示", JOptionPane.WARNING_MESSAGE);
                return;
            }
            
            try {
                int num = Integer.parseInt(input);
                if (num < 1 || num > 99999) {
                    JOptionPane.showMessageDialog(dialog, "请输入1-99999之间的整数！", 
                                                "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                }
                
                if (isPalindrome(num)) {
                    JOptionPane.showMessageDialog(dialog, num + " 是回文数！", 
                                                "结果", JOptionPane.INFORMATION_MESSAGE);
                } else {
                    JOptionPane.showMessageDialog(dialog, num + " 不是回文数！", 
                                                "结果", JOptionPane.INFORMATION_MESSAGE);
                }
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(dialog, "请输入有效的整数！", 
                                            "错误", JOptionPane.ERROR_MESSAGE);
            }
        });
        
        cancelButton.addActionListener(e -> dialog.dispose());
        
        dialog.add(panel);
        dialog.setVisible(true);
    }
    
    /**
     * 判断是否为回文数
     */
    private boolean isPalindrome(int num) {
        String str = String.valueOf(num);
        int left = 0, right = str.length() - 1;
        while (left < right) {
            if (str.charAt(left) != str.charAt(right)) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }
    
    /**
     * 数字与英文互译
     */
    private void translateNumber() {
        JDialog dialog = new JDialog(this, "数字与英文互译", true);
        dialog.setSize(500, 300);
        centerWindow(dialog);
        
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        JPanel inputPanel = new JPanel(new GridLayout(2, 2, 10, 10));
        inputPanel.add(new JLabel("输入（数字或英文）："));
        JTextField inputField = new JTextField();
        inputPanel.add(inputField);
        
        inputPanel.add(new JLabel("翻译结果："));
        JTextField outputField = new JTextField();
        outputField.setEditable(false);
        inputPanel.add(outputField);
        
        JPanel buttonPanel = new JPanel(new FlowLayout());
        JButton translateButton = new JButton("翻译");
        JButton cancelButton = new JButton("取消");
        
        buttonPanel.add(translateButton);
        buttonPanel.add(cancelButton);
        
        panel.add(inputPanel, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        translateButton.addActionListener(e -> {
            String input = inputField.getText().trim();
            if (input.isEmpty()) {
                JOptionPane.showMessageDialog(dialog, "请输入内容！", "提示", JOptionPane.WARNING_MESSAGE);
                return;
            }
            
            String result = translate(input);
            outputField.setText(result);
        });
        
        cancelButton.addActionListener(e -> dialog.dispose());
        
        dialog.add(panel);
        dialog.setVisible(true);
    }
    
    /**
     * 翻译数字和英文
     */
    private String translate(String input) {
        String[] numbers = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
        
        // 如果是数字
        if (input.matches("\\d+")) {
            int num = Integer.parseInt(input);
            if (num >= 0 && num <= 10) {
                return numbers[num];
            } else {
                return "只能翻译0-10的数字";
            }
        }
        // 如果是英文
        else {
            for (int i = 0; i < numbers.length; i++) {
                if (numbers[i].equalsIgnoreCase(input)) {
                    return String.valueOf(i);
                }
            }
            return "无法识别的英文单词";
        }
    }
    
    /**
     * 统计英文数据
     */
    private void statisticsWords() {
        String text = textArea.getText();
        if (text.isEmpty()) {
            JOptionPane.showMessageDialog(this, "请先打开或输入文本内容！", 
                                        "提示", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        // 统计26个字母开头的单词数
        int[] letterCount = new int[26];
        int orCount = 0;
        int length3Count = 0;
        
        // 提取所有单词
        Pattern pattern = Pattern.compile("[a-zA-Z]+");
        Matcher matcher = pattern.matcher(text.toLowerCase());
        
        while (matcher.find()) {
            String word = matcher.group();
            char firstChar = word.charAt(0);
            if (firstChar >= 'a' && firstChar <= 'z') {
                letterCount[firstChar - 'a']++;
            }
            
            if (word.contains("or")) {
                orCount++;
            }
            
            if (word.length() == 3) {
                length3Count++;
            }
        }
        
        // 显示统计结果
        StringBuilder result = new StringBuilder();
        result.append("统计结果：\n");
        result.append("A-Z字母开头的单词数：\n");
        for (int i = 0; i < 26; i++) {
            result.append((char)('A' + i)).append("：").append(letterCount[i]).append("\n");
        }
        result.append("\n包含'or'的单词数：").append(orCount).append("\n");
        result.append("长度为3的单词数：").append(length3Count);
        
        JTextArea resultArea = new JTextArea(result.toString());
        resultArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(resultArea);
        scrollPane.setPreferredSize(new Dimension(400, 500));
        
        JOptionPane.showMessageDialog(this, scrollPane, "统计结果", JOptionPane.INFORMATION_MESSAGE);
        
        // 显示柱状图
        showBarChart(letterCount);
    }
    
    /**
     * 显示柱状图
     */
    private void showBarChart(int[] letterCount) {
        JDialog dialog = new JDialog(this, "字母开头单词数统计图", true);
        dialog.setSize(800, 600);
        centerWindow(dialog);
        
        JPanel chartPanel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                
                int max = Arrays.stream(letterCount).max().orElse(1);
                int width = getWidth();
                int height = getHeight();
                int barWidth = (width - 100) / 26;
                int chartHeight = height - 100;
                
                // 绘制坐标轴
                g.drawLine(50, height - 50, width - 50, height - 50);
                g.drawLine(50, 50, 50, height - 50);
                
                // 绘制柱状图
                for (int i = 0; i < 26; i++) {
                    int barHeight = (int)((double)letterCount[i] / max * chartHeight);
                    int x = 50 + i * barWidth + barWidth / 4;
                    int y = height - 50 - barHeight;
                    
                    g.setColor(Color.BLUE);
                    g.fillRect(x, y, barWidth / 2, barHeight);
                    
                    g.setColor(Color.BLACK);
                    g.drawString(String.valueOf((char)('A' + i)), x + barWidth / 4, height - 30);
                    if (letterCount[i] > 0) {
                        g.drawString(String.valueOf(letterCount[i]), x + barWidth / 4, y - 5);
                    }
                }
            }
        };
        
        dialog.add(chartPanel);
        dialog.setVisible(true);
    }
    
    /**
     * 手机号码合法性判断
     */
    private void checkPhoneNumber() {
        String input = JOptionPane.showInputDialog(this, "请输入手机号码：", 
                                                 "手机号码合法性判断", JOptionPane.PLAIN_MESSAGE);
        if (input == null) return;
        
        int result = validatePhoneNumber(input.trim());
        String message;
        
        switch (result) {
            case 0:
                message = "手机号码格式合法！";
                break;
            case 1:
                message = "手机号码长度不合法！";
                break;
            case 2:
                message = "手机号码中包含非数字字符！";
                break;
            case 3:
                message = "手机号码不是以86打头！";
                break;
            default:
                message = "其他错误！";
                break;
        }
        
        JOptionPane.showMessageDialog(this, message, "验证结果", 
                                    result == 0 ? JOptionPane.INFORMATION_MESSAGE : JOptionPane.ERROR_MESSAGE);
    }
    
    /**
     * 验证手机号码合法性
     */
    private int validatePhoneNumber(String phone) {
        // 移除所有非数字字符（除了开头的+）
        String cleanPhone = phone.replaceAll("[^0-9+]", "");
        
        // 检查是否以+开头
        if (cleanPhone.startsWith("+")) {
            cleanPhone = cleanPhone.substring(1);
        }
        
        // 检查长度是否为13位
        if (cleanPhone.length() != 13) {
            return 1; // 长度不合法
        }
        
        // 检查是否全为数字
        if (!cleanPhone.matches("\\d+")) {
            return 2; // 包含非数字字符
        }
        
        // 检查是否以86开头
        if (!cleanPhone.startsWith("86")) {
            return 3; // 不是以86打头
        }
        
        return 0; // 合法
    }
    
    /**
     * 文本文件求和
     */
    private void sumFile() {
        String text = textArea.getText();
        if (text.isEmpty()) {
            JOptionPane.showMessageDialog(this, "请先打开包含变量的文本文件！", 
                                        "提示", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        // 提取所有变量
        Pattern pattern = Pattern.compile("([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*(-?\\d+\\.?\\d*)");
        Matcher matcher = pattern.matcher(text);
        
        List<String> variables = new ArrayList<>();
        List<Double> values = new ArrayList<>();
        
        while (matcher.find()) {
            variables.add(matcher.group(1));
            try {
                values.add(Double.parseDouble(matcher.group(2)));
            } catch (NumberFormatException e) {
                // 忽略无效数字
            }
        }
        
        if (variables.isEmpty()) {
            JOptionPane.showMessageDialog(this, "未找到有效的变量定义！", 
                                        "提示", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        // 显示进度条对话框
        ProgressDialog progressDialog = new ProgressDialog(this, variables, values);
        progressDialog.setVisible(true);
    }
    
    /**
     * 通讯录维护
     */
    private void maintainAddressBook() {
        AddressBookDialog dialog = new AddressBookDialog(this, addressBook);
        dialog.setVisible(true);
    }
    
    /**
     * 设置通讯录存储文件
     */
    private void setAddressBookFile() {
        JFileChooser chooser = new JFileChooser();
        chooser.setFileFilter(new FileNameExtensionFilter("数据文件", "dat"));
        
        if (chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = chooser.getSelectedFile();
            addressBook.setFile(file);
            JOptionPane.showMessageDialog(this, "通讯录存储文件已设置为：\n" + file.getAbsolutePath(), 
                                        "设置成功", JOptionPane.INFORMATION_MESSAGE);
        }
    }
    
    /**
     * 加载配置
     */
    private void loadConfig() {
        config = new Properties();
        try {
            File configFile = new File(CONFIG_FILE);
            if (configFile.exists()) {
                FileInputStream fis = new FileInputStream(configFile);
                config.load(fis);
                fis.close();
                
                currentFontName = config.getProperty("font.name", "宋体");
                currentFontSize = Integer.parseInt(config.getProperty("font.size", "16"));
                currentFontColor = new Color(Integer.parseInt(config.getProperty("font.color", "0")));
                currentBgColor = new Color(Integer.parseInt(config.getProperty("bg.color", "16777215")));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 保存配置
     */
    private void saveConfig() {
        try {
            config.setProperty("font.name", currentFontName);
            config.setProperty("font.size", String.valueOf(currentFontSize));
            config.setProperty("font.color", String.valueOf(currentFontColor.getRGB()));
            config.setProperty("bg.color", String.valueOf(currentBgColor.getRGB()));
            
            FileOutputStream fos = new FileOutputStream(CONFIG_FILE);
            config.store(fos, "Editor Configuration");
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 主方法
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            J201026810888_4 frame = new J201026810888_4();
            frame.setVisible(true);
        });
    }
}

/**
 * 字体设置对话框
 */
class FontDialog extends JDialog {
    private JComboBox<String> fontComboBox;
    private JSpinner sizeSpinner;
    private JButton colorButton;
    private JTextArea previewArea;
    private boolean ok = false;
    
    private String selectedFontName = "宋体";
    private int selectedFontSize = 16;
    private Color selectedColor = Color.BLACK;
    
    public FontDialog(JFrame parent) {
        super(parent, "字体设置", true);
        setSize(400, 300);
        J201026810888_4.centerWindow(this);
        
        initComponents();
    }
    
    private void initComponents() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        // 字体名称
        JPanel fontPanel = new JPanel(new FlowLayout());
        fontPanel.add(new JLabel("字体名称:"));
        
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        String[] fontNames = ge.getAvailableFontFamilyNames();
        fontComboBox = new JComboBox<>(fontNames);
        fontComboBox.setSelectedItem("宋体");
        fontComboBox.addActionListener(e -> {
            selectedFontName = (String)fontComboBox.getSelectedItem();
            updatePreview();
        });
        fontPanel.add(fontComboBox);
        
        // 字体大小
        JPanel sizePanel = new JPanel(new FlowLayout());
        sizePanel.add(new JLabel("字体大小:"));
        sizeSpinner = new JSpinner(new SpinnerNumberModel(16, 8, 72, 1));
        sizeSpinner.addChangeListener(e -> {
            selectedFontSize = (Integer)sizeSpinner.getValue();
            updatePreview();
        });
        sizePanel.add(sizeSpinner);
        
        // 字体颜色
        JPanel colorPanel = new JPanel(new FlowLayout());
        colorPanel.add(new JLabel("字体颜色:"));
        colorButton = new JButton("选择颜色");
        colorButton.addActionListener(e -> {
            Color color = JColorChooser.showDialog(this, "选择字体颜色", selectedColor);
            if (color != null) {
                selectedColor = color;
                updatePreview();
            }
        });
        colorPanel.add(colorButton);
        
        // 预览区域
        JPanel previewPanel = new JPanel(new BorderLayout());
        previewPanel.setBorder(BorderFactory.createTitledBorder("字体预览"));
        previewArea = new JTextArea("Java程序设计综合实验\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789");
        previewArea.setEditable(false);
        previewPanel.add(new JScrollPane(previewArea), BorderLayout.CENTER);
        
        // 按钮
        JPanel buttonPanel = new JPanel(new FlowLayout());
        JButton okButton = new JButton("确定");
        JButton cancelButton = new JButton("取消");
        
        okButton.addActionListener(e -> {
            ok = true;
            dispose();
        });
        
        cancelButton.addActionListener(e -> dispose());
        
        buttonPanel.add(okButton);
        buttonPanel.add(cancelButton);
        
        // 组装
        JPanel topPanel = new JPanel(new GridLayout(3, 1));
        topPanel.add(fontPanel);
        topPanel.add(sizePanel);
        topPanel.add(colorPanel);
        
        panel.add(topPanel, BorderLayout.NORTH);
        panel.add(previewPanel, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        add(panel);
        updatePreview();
    }
    
    private void updatePreview() {
        previewArea.setFont(new Font(selectedFontName, Font.PLAIN, selectedFontSize));
        previewArea.setForeground(selectedColor);
    }
    
    public boolean isOk() {
        return ok;
    }
    
    public String getSelectedFontName() {
        return selectedFontName;
    }
    
    public int getSelectedFontSize() {
        return selectedFontSize;
    }
    
    public Color getSelectedColor() {
        return selectedColor;
    }
}

/**
 * 进度条对话框
 */
class ProgressDialog extends JDialog {
    private JProgressBar progressBar;
    private JLabel infoLabel;
    private JButton cancelButton;
    private boolean cancelled = false;
    
    public ProgressDialog(JFrame parent, List<String> variables, List<Double> values) {
        super(parent, "计算进度", true);
        setSize(400, 200);
        J201026810888_4.centerWindow(this);
        
        initComponents();
        startCalculation(variables, values);
    }
    
    private void initComponents() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        infoLabel = new JLabel("正在计算...");
        infoLabel.setHorizontalAlignment(SwingConstants.CENTER);
        
        progressBar = new JProgressBar(0, 100);
        progressBar.setStringPainted(true);
        
        cancelButton = new JButton("取消");
        cancelButton.addActionListener(e -> {
            cancelled = true;
            dispose();
        });
        
        JPanel buttonPanel = new JPanel(new FlowLayout());
        buttonPanel.add(cancelButton);
        
        panel.add(infoLabel, BorderLayout.NORTH);
        panel.add(progressBar, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        add(panel);
    }
    
    private void startCalculation(List<String> variables, List<Double> values) {
        Thread calculationThread = new Thread(() -> {
            double sum = 0;
            int total = variables.size();
            long startTime = System.currentTimeMillis();
            
            for (int i = 0; i < total && !cancelled; i++) {
                sum += values.get(i);
                
                int progress = (int)((i + 1) * 100.0 / total);
                progressBar.setValue(progress);
                
                long elapsed = System.currentTimeMillis() - startTime;
                long remaining = (long)(elapsed * (total - i - 1.0) / (i + 1));
                
                infoLabel.setText(String.format("正在计算 %s = %.2f (%d/%d) 约剩余: %d秒", 
                                              variables.get(i), values.get(i), i + 1, total, 
                                              remaining / 1000));
                
                try {
                    Thread.sleep(100); // 模拟计算时间
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            
            if (!cancelled) {
                infoLabel.setText(String.format("计算完成！总和 = %.2f", sum));
                progressBar.setValue(100);
                cancelButton.setText("关闭");
                cancelButton.removeActionListener(cancelButton.getActionListeners()[0]);
                cancelButton.addActionListener(e -> dispose());
            }
        });
        
        calculationThread.start();
    }
}

/**
 * 通讯录联系人
 */
class Contact implements Serializable {
    private static final long serialVersionUID = 1L;
    
    private int nId;
    private String sName;
    private byte byteSex;
    private String sAddress;
    private String sCompany;
    private String sPostalCode;
    private String sHomeTele;
    private String sOfficeTele;
    private String sFax;
    private String sCellPhone;
    private String sEmail;
    private String sInstantMessager;
    private Date dateBirthday;
    private String sMemo;
    
    // 构造方法、getter和setter方法
    public Contact() {}
    
    public Contact(int nId, String sName, byte byteSex) {
        this.nId = nId;
        this.sName = sName;
        this.byteSex = byteSex;
    }
    
    // 所有字段的getter和setter方法
    public int getNId() { return nId; }
    public void setNId(int nId) { this.nId = nId; }
    
    public String getSName() { return sName; }
    public void setSName(String sName) { this.sName = sName; }
    
    public byte getByteSex() { return byteSex; }
    public void setByteSex(byte byteSex) { this.byteSex = byteSex; }
    
    public String getSAddress() { return sAddress; }
    public void setSAddress(String sAddress) { this.sAddress = sAddress; }
    
    public String getSCompany() { return sCompany; }
    public void setSCompany(String sCompany) { this.sCompany = sCompany; }
    
    public String getSPostalCode() { return sPostalCode; }
    public void setSPostalCode(String sPostalCode) { this.sPostalCode = sPostalCode; }
    
    public String getSHomeTele() { return sHomeTele; }
    public void setSHomeTele(String sHomeTele) { this.sHomeTele = sHomeTele; }
    
    public String getSOfficeTele() { return sOfficeTele; }
    public void setSOfficeTele(String sOfficeTele) { this.sOfficeTele = sOfficeTele; }
    
    public String getSFax() { return sFax; }
    public void setSFax(String sFax) { this.sFax = sFax; }
    
    public String getSCellPhone() { return sCellPhone; }
    public void setSCellPhone(String sCellPhone) { this.sCellPhone = sCellPhone; }
    
    public String getSEmail() { return sEmail; }
    public void setSEmail(String sEmail) { this.sEmail = sEmail; }
    
    public String getSInstantMessager() { return sInstantMessager; }
    public void setSInstantMessager(String sInstantMessager) { this.sInstantMessager = sInstantMessager; }
    
    public Date getDateBirthday() { return dateBirthday; }
    public void setDateBirthday(Date dateBirthday) { this.dateBirthday = dateBirthday; }
    
    public String getSMemo() { return sMemo; }
    public void setSMemo(String sMemo) { this.sMemo = sMemo; }
    
    @Override
    public String toString() {
        return sName + " (" + (byteSex == 1 ? "男" : "女") + ")";
    }
}

/**
 * 通讯录类
 */
class AddressBook implements Serializable {
    private static final long serialVersionUID = 1L;
    
    private List<Contact> contacts;
    private int nextId = 1;
    private File storageFile;
    
    public AddressBook() {
        contacts = new ArrayList<>();
        storageFile = new File("addressbook.dat");
        load();
    }
    
    public void setFile(File file) {
        this.storageFile = file;
        load();
    }
    
    public void addContact(Contact contact) {
        contact.setNId(nextId++);
        contacts.add(contact);
        save();
    }
    
    public void removeContact(Contact contact) {
        contacts.remove(contact);
        save();
    }
    
    public void updateContact(Contact contact) {
        for (int i = 0; i < contacts.size(); i++) {
            if (contacts.get(i).getNId() == contact.getNId()) {
                contacts.set(i, contact);
                break;
            }
        }
        save();
    }
    
    public List<Contact> searchContacts(String keyword) {
        List<Contact> result = new ArrayList<>();
        keyword = keyword.toLowerCase();
        
        for (Contact contact : contacts) {
            if (contact.getSName().toLowerCase().contains(keyword) ||
                (contact.getSCompany() != null && contact.getSCompany().toLowerCase().contains(keyword)) ||
                (contact.getSMemo() != null && contact.getSMemo().toLowerCase().contains(keyword))) {
                result.add(contact);
            }
        }
        
        return result;
    }
    
    public List<Contact> getAllContacts() {
        return new ArrayList<>(contacts);
    }
    
    private void save() {
        try {
            FileOutputStream fos = new FileOutputStream(storageFile);
            ObjectOutputStream oos = new ObjectOutputStream(fos);
            oos.writeObject(this);
            oos.close();
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    private void load() {
        if (storageFile.exists()) {
            try {
                FileInputStream fis = new FileInputStream(storageFile);
                ObjectInputStream ois = new ObjectInputStream(fis);
                AddressBook loaded = (AddressBook)ois.readObject();
                
                this.contacts = loaded.contacts;
                this.nextId = loaded.nextId;
                
                ois.close();
                fis.close();
            } catch (IOException | ClassNotFoundException e) {
                e.printStackTrace();
            }
        }
    }
}

/**
 * 通讯录维护对话框
 */
class AddressBookDialog extends JDialog {
    private AddressBook addressBook;
    private JTable table;
    private DefaultTableModel tableModel;
    
    public AddressBookDialog(JFrame parent, AddressBook addressBook) {
        super(parent, "通讯录维护", true);
        this.addressBook = addressBook;
        
        setSize(800, 600);
        J201026810888_4.centerWindow(this);
        
        initComponents();
        refreshTable();
    }
    
    private void initComponents() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        // 创建表格
        String[] columnNames = {"序号", "姓名", "性别", "地址", "单位", "邮编", "家庭电话", "办公电话", "传真", "手机", "邮箱", "即时通讯", "生日", "备注"};
        tableModel = new DefaultTableModel(columnNames, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };
        
        table = new JTable(tableModel);
        JScrollPane scrollPane = new JScrollPane(table);
        
        // 按钮面板
        JPanel buttonPanel = new JPanel(new FlowLayout());
        JButton addButton = new JButton("添加");
        JButton deleteButton = new JButton("删除");
        JButton modifyButton = new JButton("修改");
        JButton searchButton = new JButton("查询");
        JButton closeButton = new JButton("关闭");
        
        addButton.addActionListener(e -> addContact());
        deleteButton.addActionListener(e -> deleteContact());
        modifyButton.addActionListener(e -> modifyContact());
        searchButton.addActionListener(e -> searchContacts());
        closeButton.addActionListener(e -> dispose());
        
        buttonPanel.add(addButton);
        buttonPanel.add(deleteButton);
        buttonPanel.add(modifyButton);
        buttonPanel.add(searchButton);
        buttonPanel.add(closeButton);
        
        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        add(panel);
    }
    
    private void refreshTable() {
        tableModel.setRowCount(0);
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        for (Contact contact : addressBook.getAllContacts()) {
            Object[] row = {
                contact.getNId(),
                contact.getSName(),
                contact.getByteSex() == 1 ? "男" : "女",
                contact.getSAddress(),
                contact.getSCompany(),
                contact.getSPostalCode(),
                contact.getSHomeTele(),
                contact.getSOfficeTele(),
                contact.getSFax(),
                contact.getSCellPhone(),
                contact.getSEmail(),
                contact.getSInstantMessager(),
                contact.getDateBirthday() != null ? sdf.format(contact.getDateBirthday()) : "",
                contact.getSMemo()
            };
            tableModel.addRow(row);
        }
    }
    
    private void addContact() {
        ContactEditDialog dialog = new ContactEditDialog(this, null);
        dialog.setVisible(true);
        
        if (dialog.isOk()) {
            Contact contact = dialog.getContact();
            addressBook.addContact(contact);
            refreshTable();
        }
    }
    
    private void deleteContact() {
        int selectedRow = table.getSelectedRow();
        if (selectedRow == -1) {
            JOptionPane.showMessageDialog(this, "请选择要删除的联系人！", 
                                        "提示", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        String name = (String)tableModel.getValueAt(selectedRow, 1);
        int result = JOptionPane.showConfirmDialog(this, "确定要删除联系人 '" + name + "' 吗？", 
                                                "确认删除", JOptionPane.YES_NO_OPTION);
        
        if (result == JOptionPane.YES_OPTION) {
            int id = (Integer)tableModel.getValueAt(selectedRow, 0);
            for (Contact contact : addressBook.getAllContacts()) {
                if (contact.getNId() == id) {
                    addressBook.removeContact(contact);
                    break;
                }
            }
            refreshTable();
        }
    }
    
    private void modifyContact() {
        int selectedRow = table.getSelectedRow();
        if (selectedRow == -1) {
            JOptionPane.showMessageDialog(this, "请选择要修改的联系人！", 
                                        "提示", JOptionPane.WARNING_MESSAGE);
            return;
        }
        
        int id = (Integer)tableModel.getValueAt(selectedRow, 0);
        Contact contact = null;
        for (Contact c : addressBook.getAllContacts()) {
            if (c.getNId() == id) {
                contact = c;
                break;
            }
        }
        
        if (contact != null) {
            ContactEditDialog dialog = new ContactEditDialog(this, contact);
            dialog.setVisible(true);
            
            if (dialog.isOk()) {
                addressBook.updateContact(dialog.getContact());
                refreshTable();
            }
        }
    }
    
    private void searchContacts() {
        String keyword = JOptionPane.showInputDialog(this, "请输入姓名、单位或备注的关键字：", 
                                                   "查询联系人", JOptionPane.PLAIN_MESSAGE);
        if (keyword == null || keyword.trim().isEmpty()) {
            refreshTable();
            return;
        }
        
        tableModel.setRowCount(0);
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        for (Contact contact : addressBook.searchContacts(keyword.trim())) {
            Object[] row = {
                contact.getNId(),
                contact.getSName(),
                contact.getByteSex() == 1 ? "男" : "女",
                contact.getSAddress(),
                contact.getSCompany(),
                contact.getSPostalCode(),
                contact.getSHomeTele(),
                contact.getSOfficeTele(),
                contact.getSFax(),
                contact.getSCellPhone(),
                contact.getSEmail(),
                contact.getSInstantMessager(),
                contact.getDateBirthday() != null ? sdf.format(contact.getDateBirthday()) : "",
                contact.getSMemo()
            };
            tableModel.addRow(row);
        }
    }
}

/**
 * 联系人编辑对话框
 */
class ContactEditDialog extends JDialog {
    private Contact contact;
    private boolean ok = false;
    
    private JTextField nameField;
    private JRadioButton maleButton, femaleButton;
    private JTextField addressField, companyField, postalField;
    private JTextField homeTeleField, officeTeleField, faxField;
    private JTextField cellPhoneField, emailField, imField;
    private JTextField birthdayField, memoField;
    
    public ContactEditDialog(JDialog parent, Contact contact) {
        super(parent, contact == null ? "添加联系人" : "修改联系人", true);
        this.contact = contact;
        
        setSize(500, 600);
        J201026810888_4.centerWindow(this);
        
        initComponents();
        
        if (contact != null) {
            loadContactData();
        }
    }
    
    private void initComponents() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        // 输入面板
        JPanel inputPanel = new JPanel(new GridLayout(14, 2, 10, 5));
        
        // 姓名（必填）
        inputPanel.add(new JLabel("姓名（*）："));
        nameField = new JTextField();
        inputPanel.add(nameField);
        
        // 性别（必填）
        inputPanel.add(new JLabel("性别（*）："));
        JPanel sexPanel = new JPanel(new FlowLayout());
        maleButton = new JRadioButton("男", true);
        femaleButton = new JRadioButton("女");
        ButtonGroup group = new ButtonGroup();
        group.add(maleButton);
        group.add(femaleButton);
        sexPanel.add(maleButton);
        sexPanel.add(femaleButton);
        inputPanel.add(sexPanel);
        
        // 其他字段
        inputPanel.add(new JLabel("地址："));
        addressField = new JTextField();
        inputPanel.add(addressField);
        
        inputPanel.add(new JLabel("单位："));
        companyField = new JTextField();
        inputPanel.add(companyField);
        
        inputPanel.add(new JLabel("邮编："));
        postalField = new JTextField();
        inputPanel.add(postalField);
        
        inputPanel.add(new JLabel("家庭电话："));
        homeTeleField = new JTextField();
        inputPanel.add(homeTeleField);
        
        inputPanel.add(new JLabel("办公电话："));
        officeTeleField = new JTextField();
        inputPanel.add(officeTeleField);
        
        inputPanel.add(new JLabel("传真："));
        faxField = new JTextField();
        inputPanel.add(faxField);
        
        inputPanel.add(new JLabel("手机："));
        cellPhoneField = new JTextField();
        inputPanel.add(cellPhoneField);
        
        inputPanel.add(new JLabel("邮箱："));
        emailField = new JTextField();
        inputPanel.add(emailField);
        
        inputPanel.add(new JLabel("即时通讯："));
        imField = new JTextField();
        inputPanel.add(imField);
        
        inputPanel.add(new JLabel("生日（yyyy-MM-dd）："));
        birthdayField = new JTextField();
        inputPanel.add(birthdayField);
        
        inputPanel.add(new JLabel("备注："));
        memoField = new JTextField();
        inputPanel.add(memoField);
        
        // 按钮面板
        JPanel buttonPanel = new JPanel(new FlowLayout());
        JButton okButton = new JButton("确定");
        JButton cancelButton = new JButton("取消");
        
        okButton.addActionListener(e -> {
            if (validateInput()) {
                ok = true;
                if (contact == null) {
                    contact = new Contact();
                }
                saveContactData();
                dispose();
            }
        });
        
        cancelButton.addActionListener(e -> dispose());
        
        buttonPanel.add(okButton);
        buttonPanel.add(cancelButton);
        
        panel.add(inputPanel, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        add(panel);
    }
    
    private void loadContactData() {
        nameField.setText(contact.getSName());
        if (contact.getByteSex() == 1) {
            maleButton.setSelected(true);
        } else {
            femaleButton.setSelected(true);
        }
        addressField.setText(contact.getSAddress());
        companyField.setText(contact.getSCompany());
        postalField.setText(contact.getSPostalCode());
        homeTeleField.setText(contact.getSHomeTele());
        officeTeleField.setText(contact.getSOfficeTele());
        faxField.setText(contact.getSFax());
        cellPhoneField.setText(contact.getSCellPhone());
        emailField.setText(contact.getSEmail());
        imField.setText(contact.getSInstantMessager());
        birthdayField.setText(contact.getDateBirthday() != null ? 
                            new SimpleDateFormat("yyyy-MM-dd").format(contact.getDateBirthday()) : "");
        memoField.setText(contact.getSMemo());
    }
    
    private void saveContactData() {
        contact.setSName(nameField.getText().trim());
        contact.setByteSex(maleButton.isSelected() ? (byte)1 : (byte)0);
        contact.setSAddress(addressField.getText().trim());
        contact.setSCompany(companyField.getText().trim());
        contact.setSPostalCode(postalField.getText().trim());
        contact.setSHomeTele(homeTeleField.getText().trim());
        contact.setSOfficeTele(officeTeleField.getText().trim());
        contact.setSFax(faxField.getText().trim());
        contact.setSCellPhone(cellPhoneField.getText().trim());
        contact.setSEmail(emailField.getText().trim());
        contact.setSInstantMessager(imField.getText().trim());
        
        String birthdayStr = birthdayField.getText().trim();
        if (!birthdayStr.isEmpty()) {
            try {
                Date birthday = new SimpleDateFormat("yyyy-MM-dd").parse(birthdayStr);
                contact.setDateBirthday(birthday);
            } catch (Exception e) {
                // 忽略无效日期
            }
        }
        
        contact.setSMemo(memoField.getText().trim());
    }
    
    private boolean validateInput() {
        String name = nameField.getText().trim();
        if (name.isEmpty()) {
            JOptionPane.showMessageDialog(this, "姓名不能为空！", "错误", JOptionPane.ERROR_MESSAGE);
            nameField.requestFocus();
            return false;
        }
        return true;
    }
    
    public boolean isOk() {
        return ok;
    }
    
    public Contact getContact() {
        return contact;
    }
}
