### 【2417】report user defined symbol data 报告用户自定义符号数据

#### 代码

```cpp
    static void report_user_defined_symbol_data(tag_t uds)  
    {  
        UF_DRF_symbol_data_t  
            symbol_data;  
        UF_CALL(UF_DISP_set_highlight(uds, TRUE));  
        UF_CALL(UF_DRF_ask_symbol_data(uds, &symbol_data));  
        show_objects_csys(uds, symbol_data.anchor_point);  
        ECHO3F(symbol_data.anchor_point);  
        display_temporary_point(symbol_data.orient_point);  
        ECHO3F(symbol_data.orient_point);  
        ECHOF(symbol_data.angle);  
        ECHOF(symbol_data.length);  
        ECHOF(symbol_data.height);  
        ECHO(symbol_data.symbol_font_tag);  
        uc1601("OK?", TRUE);  
        UF_CALL(UF_DISP_set_highlight(uds, FALSE));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，主要功能是报告用户定义符号的数据。
>
> 代码的主要步骤如下：
>
> 1. 定义一个符号数据结构体，用于存储符号的各种数据，包括锚点、方向点、角度、长度、高度和符号字体标签。
> 2. 使用UF_DISP_set_highlight函数高亮显示用户定义的符号。
> 3. 使用UF_DRF_ask_symbol_data函数查询符号的数据，并存储到symbol_data结构体中。
> 4. 使用show_objects_csys函数显示符号的锚点坐标系。
> 5. 使用display_temporary_point函数显示符号的方向点。
> 6. 打印符号的方向点、角度、长度、高度和符号字体标签等信息。
> 7. 使用uc1601函数弹出一个对话框，提示用户“OK?”。
> 8. 最后，取消高亮显示符号。
>
> 通过这些步骤，代码实现了查询和报告用户定义符号的数据的功能。
>
