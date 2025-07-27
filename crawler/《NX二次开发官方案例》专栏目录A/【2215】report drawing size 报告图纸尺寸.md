### 【2215】report drawing size 报告图纸尺寸

#### 代码

```cpp
    static void report_drawing_size(tag_t drawing)  
    {  
        char  
            draw_name[MAX_ENTITY_NAME_SIZE+1],  
            msg[MAX_LINE_SIZE+1],  
            *unit_names[2] = { "mm", "inches" };  
        UF_DRAW_info_t  
            info;  
        UF_UI_open_listing_window();  
        UF_CALL(UF_OBJ_ask_name(drawing, draw_name));  
        UF_UI_write_listing_window(draw_name);  
        UF_CALL(UF_DRAW_ask_drawing_info(drawing, &info));  
        switch (info.size_state)  
        {  
            case UF_DRAW_METRIC_SIZE:  
                switch (info.size.metric_size_code)  
                {  
                    case UF_DRAW_A0:  
                        UF_UI_write_listing_window(" - A0 (841 x 1189 mm)\n");  
                        break;  
                    case UF_DRAW_A1:  
                        UF_UI_write_listing_window(" - A1 (594 x 841 mm)\n");  
                        break;  
                    case UF_DRAW_A2:  
                        UF_UI_write_listing_window(" - A2 (420 x 594 mm)\n");  
                        break;  
                    case UF_DRAW_A3:  
                        UF_UI_write_listing_window(" - A3 (297 x 420 mm)\n");  
                        break;  
                    case UF_DRAW_A4:  
                        UF_UI_write_listing_window(" - A4 (210 x 297 mm)\n");  
                        break;  
                    default:  
                        UF_UI_write_listing_window(" - Unknown metric size\n");  
                        break;  
                }  
                break;  
            case UF_DRAW_ENGLISH_SIZE:  
                switch (info.size.english_size_code)  
                {  
                    case UF_DRAW_A:  
                        UF_UI_write_listing_window(" - A (8.5 x 11 inches)\n");  
                        break;  
                    case UF_DRAW_B:  
                        UF_UI_write_listing_window(" - B (11 x 17 inches)\n");  
                        break;  
                    case UF_DRAW_C:  
                        UF_UI_write_listing_window(" - C (17 x 22 inches)\n");  
                        break;  
                    case UF_DRAW_D:  
                        UF_UI_write_listing_window(" - D (22 x 34 inches)\n");  
                        break;  
                    case UF_DRAW_E:  
                        UF_UI_write_listing_window(" - E (34 x 44 inches)\n");  
                        break;  
                    case UF_DRAW_F:  
                        UF_UI_write_listing_window(" - F (28 x 40 inches)\n");  
                        break;  
                    case UF_DRAW_H:  
                        UF_UI_write_listing_window(" - H (28 x 44 inches)\n");  
                        break;  
                    case UF_DRAW_J:  
                        UF_UI_write_listing_window(" - J (34 x 55 inches)\n");  
                        break;  
                    default:  
                        UF_UI_write_listing_window(" - Unknown english size\n");  
                        break;  
                }  
                break;  
            case UF_DRAW_CUSTOM_SIZE:  
                sprintf(msg, " - Custom size (%.1f x %.1f %s)\n",  
                    info.size.custom_size[0], info.size.custom_size[1],  
                    unit_names[info.units - 1]);  
                UF_UI_write_listing_window(msg);  
                break;  
            default:  
                UF_UI_write_listing_window(" - Unknown size\n");  
                break;  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是报告图纸的尺寸信息。具体步骤如下：
>
> 1. 首先打开一个列表窗口用于显示信息。
> 2. 获取当前图纸的名称，并显示在列表窗口中。
> 3. 使用UF_DRAW_ask_drawing_info函数查询当前图纸的尺寸信息，包括尺寸状态、尺寸代码等。
> 4. 根据图纸的尺寸状态（公制/英制/自定义）和尺寸代码，显示图纸的具体尺寸，例如A0、A1、A2等。
> 5. 对于自定义尺寸，需要额外显示宽度和高度值，以及单位信息。
> 6. 如果无法识别图纸的尺寸状态或代码，则显示未知尺寸信息。
> 7. 最后关闭列表窗口。
>
> 通过这段代码，用户可以清晰地了解当前图纸的尺寸规格，从而方便进行后续的出图操作。
>
