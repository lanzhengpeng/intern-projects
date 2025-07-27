### 【2149】report id symbol type 报告ID符号类型

#### 代码

```cpp
    static void report_id_symbol_type(UF_DRF_id_symbol_type_t id_symbol_type)  
    {  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_UI_write_listing_window("id_symbol_type = "));  
        switch (id_symbol_type)  
        {  
            case UF_DRF_sym_circle:  
                WRITE_D(UF_DRF_sym_circle);  
                break;  
            case UF_DRF_sym_divcir:  
                WRITE_D(UF_DRF_sym_divcir);  
                break;  
            case UF_DRF_sym_square:  
                WRITE_D(UF_DRF_sym_square);  
                break;  
            case UF_DRF_sym_divsqr:  
                WRITE_D(UF_DRF_sym_divsqr);  
                break;  
            case UF_DRF_sym_hexagon:  
                WRITE_D(UF_DRF_sym_hexagon);  
                break;  
            case UF_DRF_sym_divhex:  
                WRITE_D(UF_DRF_sym_divhex);  
                break;  
            case UF_DRF_sym_triup:  
                WRITE_D(UF_DRF_sym_triup);  
                break;  
            case UF_DRF_sym_tridown:  
                WRITE_D(UF_DRF_sym_tridown);  
                break;  
            case UF_DRF_sym_datum:  
                WRITE_D(UF_DRF_sym_datum);  
                break;  
            case UF_DRF_sym_roundbox:  
                WRITE_D(UF_DRF_sym_roundbox);  
                break;  
            default:  
                UF_CALL(UF_UI_write_listing_window("Unknown type\n"));  
                WRITE_D(id_symbol_type);  
                break;  
        }  
    }

```

#### 代码解析

> 这是一段用于在NX中读取和输出DRF符号类型的二次开发代码，主要功能包括：
>
> 1. 定义了一个静态函数report_id_symbol_type，用于报告DRF符号的类型。
> 2. 使用UF_UI_open_listing_window函数打开NX的列表窗口。
> 3. 使用UF_UI_write_listing_window函数在列表窗口中输出"id_symbol_type = "。
> 4. 通过switch语句，根据传入的id_symbol_type参数值，输出对应的DRF符号类型名称。
> 5. 使用WRITE_D宏输出参数值，以便调试。
> 6. 如果传入的id_symbol_type参数不是已知的DRF符号类型，则输出"Unknown type"。
> 7. 该函数通过读取NX DRF文件，获取符号类型信息，并在列表窗口中输出，方便用户查看DRF文件中的符号类型。
>
> 这段代码主要用于NX二次开发中处理DRF文件，获取符号类型信息，并通过输出在列表窗口中展示，为后续处理DRF文件提供支持。
>
