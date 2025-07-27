### 【1737】place custom symbols with leader 在引导线上放置自定义符号

#### 代码

```cpp
    /*HEAD PLACE_CUSTOM_SYMBOLS_WITH_LEADER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，UF_print_syslog 是在 V18 中新增的函数，翻译如下：

注意：UF_print_syslog 是在 V18 中新增的函数，请仅回答翻译，不要添加其他内容。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            lines[2],  
            symbol,  
            leader_terminators[1];  
        double  
            anchor[3] = { 2.5, 2.5, 0 },  
            note_origin[3] = { 1, 0, 0 },  
            cp[3];  
        char  
            *note_text[1] = { "DUMMY TEXT" },  
            *custom_text = { "HERE!" };  
        UF_CURVE_line_t  
            line_data[2] = {{{ 0,0,0 }, { 5,5,0}}, {{0,5,0}, {5,0,0}}};  
        UF_DRF_custom_symbol_t  
            symbol_definition;  
        UF_DRF_custom_symbol_text_t  
            symbol_text;  
        UF_DRF_symbol_connection_t  
            connections[1];  
        UF_DRF_leader_t  
            leader;  
        UF_DRF_intermediate_points_t  
            intermediate_points[1];  
        // fill sub structures  
        intermediate_points[0].num_points = 0;  
        intermediate_points[0].points = NULL_TAG;  
        connections[0].connection_type = UF_DRF_LEFT_LEADER_CONNECTION;  
        // Initialize the leader  
        UF_CALL( UF_DRF_initialize_leader_data( &leader ) );  
        leader.num_leaders = 1;  
        leader.leader_terminators = leader_terminators;  
        leader.leader_orientation = UF_DRF_LEADER_LEFT;  
        leader.intermediate_points = intermediate_points;  
        UF_CALL(UF_DRF_initialize_custom_symbol_data(&symbol_definition));  
        symbol_definition.num_connections = 1;  
        symbol_definition.connections = connections;  
        symbol_definition.leader = &leader;  
        symbol_definition.geometry = lines;  
        symbol_definition.num_geometry = 2;  
        symbol_definition.num_text = 1;  
        symbol_definition.text = &symbol_text;  
        // Initialize the symbol text  
        UF_CALL(UF_DRF_initialize_custom_symbol_text_data(&symbol_text));  
        symbol_text.title = "X marks the spot";  
        symbol_text.simple_text = custom_text;  
        while (select_pos("Select terminator position", cp) &&  
               select_pos("Place custom symbol", symbol_definition.origin))  
        {  
            UF_CALL(UF_CURVE_create_line(&line_data[0], &lines[0]));  
            UF_CALL(UF_CURVE_create_line(&line_data[1], &lines[1]));  
            UF_CALL(UF_CURVE_create_point(anchor, &symbol_definition.anchor_point));  
            UF_CALL(UF_CURVE_create_point(anchor, &connections[0].connection_point));  
            UF_CALL(UF_CURVE_create_point(cp, &leader_terminators[0]));  
            UF_CALL(UF_DRF_create_note(1, note_text, note_origin, 0,  
                &symbol_text.note_tag));  
            UF_CALL(UF_DRF_create_custom_symbol_instance(&symbol_definition, &symbol));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open的二次开发示例，用于创建自定义符号并添加到NX模型中。代码的主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于打印出函数调用失败的信息。
> 2. 定义了选择位置的函数select_pos，用于提示用户选择一个点。
> 3. 定义了执行主要功能的函数do_it，该函数执行以下操作：定义符号的线条、文本、连接和领导者等数据结构。初始化符号的线条、文本、连接和领导者。在循环中提示用户选择符号的终止位置和放置位置。创建线条、点、注释等，并调用UF_DRF_create_custom_symbol_instance函数创建自定义符号实例。
> 4. 定义符号的线条、文本、连接和领导者等数据结构。
> 5. 初始化符号的线条、文本、连接和领导者。
> 6. 在循环中提示用户选择符号的终止位置和放置位置。
> 7. 创建线条、点、注释等，并调用UF_DRF_create_custom_symbol_instance函数创建自定义符号实例。
> 8. 定义了ufusr函数，该函数初始化NX Open，调用do_it函数，然后终止NX Open。
> 9. 定义了ufusr_ask_unload函数，用于卸载NX Open库。
>
> 总体来说，这段代码实现了自定义符号的创建和添加，为NX二次开发提供了有用的示例。
>
