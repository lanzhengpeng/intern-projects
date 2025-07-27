### 【1124】display objects bounding box 显示对象边界框

#### 代码

```cpp
    static void display_objects_bounding_box(tag_t object)  
    {  
        double  
            box[6],  
            end[3],  
            start[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_MODL_ask_bounding_box(object, box));  
        report_double_array("box", box, 2, 3);  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是显示一个NX对象（如实体、曲面等）的边界框。
>
> 关键步骤如下：
>
> 1. 使用UF_MODL_ask_bounding_box函数获取对象的边界框信息，存放在box数组中，其中box[0]到box[2]表示边界框的起始点，box[3]到box[5]表示边界框的结束点。
> 2. 代码通过循环遍历边界框的8个顶点，使用UF_DISP_display_temporary_line函数绘制边界框的12条边。每条边的两个端点由start数组和end数组指定。
> 3. 在绘制每条边之前，代码设置了一些显示属性，如颜色、线型等。
> 4. 通过不断改变start和end数组中的值，代码遍历边界框的所有顶点，绘制出完整的边界框。
> 5. 边界框的显示是临时的，不会保存到模型中，关闭NX后消失。
>
> 总体来说，这段代码利用NX的API函数，实现了显示对象边界框的功能，并考虑了细节，如设置显示属性等。
>
