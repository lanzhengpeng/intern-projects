### 【0719】create id symbols 创建ID符号

#### 代码

```cpp
    static void create_id_symbols(void)  
    {  
        char  
            dname[35],  
            low_text[MAX_ID_SYM_TEXT_LENGTH],  
            upp_text[MAX_ID_SYM_TEXT_LENGTH];  
        int  
            i = 0;  
        double  
            origin[3],  
            model_pos[3];  
        tag_t  
            idsym;  
        UF_DRF_object_t  
            curve;  
        UF_CALL(UF_DRF_init_object_structure(&curve));  
        while ((select_drf_curve(&curve, model_pos)) &&   
               (uc1615("Ind. id symbol position", origin)) == 5)  
        {  
            sprintf(upp_text, "%d", ++i);  
            UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, upp_text,  
                low_text, origin, UF_DRF_with_leader,  
                UF_DRF_leader_attach_object, &curve, model_pos, &idsym));  
            map_model2dwg(model_pos, curve.object_view_tag);  
            sprintf(upp_text, "%d", ++i);  
            UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, upp_text,  
                low_text, origin, UF_DRF_with_leader,  
                UF_DRF_leader_attach_object, &curve, model_pos, &idsym));  
        }  
    }

```

#### 代码解析

> 这是段用于在NX模型中批量创建标识符号的二次开发代码。
>
> 主要步骤包括：
>
> 1. 初始化DRF对象结构。
> 2. 循环选择模型中的曲线，并获取曲线的模型坐标。
> 3. 提示用户输入标识符号的放置位置。
> 4. 在输入的位置，创建一个圆形的标识符号，并设置其文本为递增的数字。标识符号带有一个指向曲线的引线。
> 5. 调用函数将曲线的模型坐标映射到图纸坐标。
> 6. 重复步骤2-5，直到没有更多曲线可选择。
> 7. 每次循环都会创建两个标识符号，一个带引线，一个不带引线。
>
> 这段代码的主要作用是在NX模型中自动批量创建标识符号，以提高绘图效率。
>
