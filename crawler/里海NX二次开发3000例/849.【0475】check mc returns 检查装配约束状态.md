### 【0475】check mc returns 检查装配约束状态

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void check_mc_returns(UF_ASSEM_mc_structure_state_t struct_state,  
        UF_ASSEM_mc_status_t stat)  
    {  
        WRITE("struct_state = ");  
        switch(struct_state)  
        {  
            case UF_ASSEM_structure_ok:  
                WRITE_D(UF_ASSEM_structure_ok);  
                break;  
            case UF_ASSEM_no_component:  
                WRITE_D(UF_ASSEM_no_component);  
                break;  
            case UF_ASSEM_invalid_entity_type:  
                WRITE_D(UF_ASSEM_invalid_entity_type);  
                break;  
            case UF_ASSEM_invalid_occ:  
                WRITE_D(UF_ASSEM_invalid_occ);  
                break;  
            case UF_ASSEM_invalid_from_part:  
                WRITE_D(UF_ASSEM_invalid_from_part);  
                break;  
            case UF_ASSEM_invalid_to_part:  
                WRITE_D(UF_ASSEM_invalid_to_part);  
                break;  
            case UF_ASSEM_circular_mating:  
                WRITE_D(UF_ASSEM_circular_mating);  
                break;  
            case UF_ASSEM_promoted_object:  
                WRITE_D(UF_ASSEM_promoted_object);  
                break;  
            case UF_ASSEM_invalid_offset:  
                WRITE_D(UF_ASSEM_invalid_offset);  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(struct_state);  
                break;  
        }  
        WRITE("stat = ");  
        switch(stat)  
        {  
            case UF_ASSEM_mc_solved:  
                WRITE_D(UF_ASSEM_mc_solved);  
                break;  
            case UF_ASSEM_mc_not_solved:  
                WRITE_D(UF_ASSEM_mc_not_solved);  
                break;  
            case UF_ASSEM_mc_no_mating_specified:  
                WRITE_D(UF_ASSEM_mc_no_mating_specified);  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(stat);  
                break;  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于检查并输出结构状态和装配状态。
>
> 主要内容包括：
>
> 1. 定义了WRITE和WRITE_D两个宏，用于打开列表窗口并写入文本。
> 2. 定义了write_integer_to_listing_window函数，用于将整数写入列表窗口。
> 3. 定义了check_mc_returns函数，用于检查结构状态和装配状态，并输出。
> 4. 在check_mc_returns函数中，通过switch语句判断结构状态，并输出对应的枚举值。
> 5. 同样在check_mc_returns函数中，通过另一个switch语句判断装配状态，并输出对应的枚举值。
> 6. 使用了NX Open提供的UF_ASSEM结构状态和装配状态的枚举类型。
> 7. 通过UF_UI函数打开列表窗口，并在窗口中输出文本。
> 8. 使用了C语言的sprintf函数格式化字符串。
> 9. 整体代码结构清晰，通过宏和函数封装提高了代码复用性。
> 10. 通过输出结构状态和装配状态，有助于调试和了解装配体的情况。
>
> 总体来说，这段代码简洁高效，通过封装提高了可读性和可维护性，有助于NX装配体状态的检查和输出。
>
