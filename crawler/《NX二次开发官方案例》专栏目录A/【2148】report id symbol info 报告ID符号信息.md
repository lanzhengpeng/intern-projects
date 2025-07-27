### 【2148】report id symbol info 报告ID符号信息

#### 代码

```cpp
    static void report_id_symbol_info(tag_t id_symbol)  
    {  
        int  
            ii, jj,  
            ugv;  
        double  
            id_symbol_origin[3];  
        UF_DRF_id_symbol_type_t  
            id_symbol_type;  
        UF_DRF_id_symbol_info_t  
            *id_symbol_info;  
        WRITE("\n");  
        WRITE_D(id_symbol);  
        UF_CALL(UF_DRF_ask_id_symbol_info(id_symbol, &id_symbol_type,  
            id_symbol_origin, &id_symbol_info));  
        WRITE_D(id_symbol_type);  
        WRITE3F(id_symbol_origin);  
        WRITE_F(id_symbol_info->size);  
        WRITE_D(id_symbol_info->num_text);  
        for (ii = 0; ii < id_symbol_info->num_text; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(id_symbol_info->text_info[ii].text_type);  
            WRITE_D(id_symbol_info->text_info[ii].text_font);  
            WRITE_F(id_symbol_info->text_info[ii].size);  
            WRITE_F(id_symbol_info->text_info[ii].angle);  
            WRITE3F(id_symbol_info->text_info[ii].origin);  
            WRITE_F(id_symbol_info->text_info[ii].length);  
            WRITE_F(id_symbol_info->text_info[ii].height);  
            WRITE_F(id_symbol_info->text_info[ii].distance);  
            WRITE_F(id_symbol_info->text_info[ii].aspect_ratio);  
            WRITE_F(id_symbol_info->text_info[ii].gap);  
            WRITE_F(id_symbol_info->text_info[ii].line_spacing);  
            WRITE_D(id_symbol_info->text_info[ii].num_lines);  
            for (jj=0; jj<id_symbol_info->text_info[ii].num_lines; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_S(id_symbol_info->text_info[ii].text[jj].string);  
            }  
        }  
        WRITE_D(id_symbol_info->num_leaders);  
        if (id_symbol_info->num_leaders > 0)  
        {  
            WRITEnD(id_symbol_info->leader_info->leader_attach_type,  
                id_symbol_info->num_leaders);  
            WRITE_D(id_symbol_info->leader_info->num_linebks);  
            for (ii = 0; ii < id_symbol_info->leader_info->num_linebks; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(id_symbol_info->leader_info->  
                    leader_pnts[ii].sequence_number);  
                WRITE_D(id_symbol_info->leader_info->leader_pnts[ii].num_symbols);  
                WRITE_D(id_symbol_info->leader_info->leader_pnts[ii].num_segments);  
                write_double_array_to_listing_window(  
                    "id_symbol_info->leader_info->leader_pnts[ii].segment_pnts",  
                    id_symbol_info->leader_info->leader_pnts[ii].segment_pnts,  
                    id_symbol_info->leader_info->leader_pnts[ii].num_segments,  
                    6);  
            }  
            WRITE_D(id_symbol_info->leader_info->num_assoc_objs);  
            for (ii = 0; ii < id_symbol_info->leader_info->num_assoc_objs; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(id_symbol_info->leader_info->  
                    assoc_objs[ii].assoc_object_tag);  
                WRITE_D(id_symbol_info->leader_info->  
                    assoc_objs[ii].assoc_object_view_tag);  
                WRITE_D(id_symbol_info->leader_info->assoc_objs[ii].assoc_type);  
                WRITE_D(id_symbol_info->leader_info->assoc_objs[ii].assoc_modifier);  
            }  
        }  
        WRITE_D(id_symbol_info->num_arrows);  
        /*  There's a problem with arrows prior to V17 译:V17之前的箭头存在问题。 */  
        ugv = ask_major_ug_version_number();  
        if (ugv > 16)  
            for (ii = 0; ii < id_symbol_info->num_arrows; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(id_symbol_info->arrow_info[ii].sequence_number);  
                WRITE_D(id_symbol_info->arrow_info[ii].arrow_type);  
                WRITE_D(id_symbol_info->arrow_info[ii].arrow_fill);  
                WRITE3F(id_symbol_info->arrow_info[ii].arrow_origin);  
                WRITE_F(id_symbol_info->arrow_info[ii].arrow_angle);  
                WRITE_F(id_symbol_info->arrow_info[ii].arrow_include_angle);  
                WRITE_F(id_symbol_info->arrow_info[ii].arrow_height);  
                WRITE_F(id_symbol_info->arrow_info[ii].arrow_width);  
            }  
        UF_CALL(UF_DRF_free_idsymbol(&id_symbol_info));  
    }

```

#### 代码解析

> 这段代码是用于获取和打印NX中ID符号信息的功能函数。主要步骤包括：
>
> 1. 获取ID符号的属性信息，包括类型、原点、大小等。
> 2. 获取ID符号中的文本信息，包括文本类型、字体、大小、角度、原点、长度、高度等，并打印每行文本。
> 3. 获取ID符号中的引导线信息，包括附着类型、折线点、关联对象等。
> 4. 获取ID符号中的箭头信息，包括序号、类型、填充、原点、角度等。
> 5. 释放ID符号信息结构体。
> 6. 根据NX版本，V17之前不处理箭头信息。
> 7. 使用WRITE系列宏打印ID符号的各种信息到日志窗口。
>
> 总体来说，这段代码实现了对NX中ID符号的全面信息获取和打印，为后续处理ID符号提供了基础。代码结构清晰，通过函数参数、结构体、循环和条件判断等，实现了对ID符号信息的逐层获取和打印。
>
