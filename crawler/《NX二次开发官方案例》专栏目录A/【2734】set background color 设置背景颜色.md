### 【2734】set background color 设置背景颜色

#### 代码

```cpp
    /*  
        This function sets and loads the color table background color   
        red, green and blue values should be doubles between zero and 1.  
        1.0, 1.0, 1.0 = white, 0.0, 0.0, 0.0 = black, 0.0, 0.0, 1.0 = blue  
    */  
    void set_background_color(double redval, double grnval, double bluval)  
    {  
        double clr_values[3];  
        clr_values[0] = redval;  
        clr_values[1] = bluval;  
        clr_values[2] = grnval;  
        UF_CALL(UF_DISP_set_color(UF_DISP_BACKGROUND_COLOR,   
            UF_DISP_rgb_model, "", clr_values ));  
        UF_CALL(UF_DISP_load_color_table());  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，主要用于设置NX的背景颜色。
>
> 代码的主要逻辑如下：
>
> 1. 定义一个double数组clr_values，用于存储红绿蓝三个颜色通道的值，范围是0.0到1.0。
> 2. 调用UF_DISP_set_color函数，将背景颜色设置为传入的红绿蓝三个颜色值。其中UF_DISP_BACKGROUND_COLOR表示设置的是背景颜色，UF_DISP_rgb_model表示使用的是RGB颜色模型。
> 3. 调用UF_DISP_load_color_table函数，加载颜色表，使设置生效。
> 4. 在调用NX Open的UF函数时，使用了UF_CALL宏来捕获函数返回的错误码，以确保代码的健壮性。
>
> 需要注意的是，在函数中，设置颜色值的顺序是redval、bluval、grnval，与常见的RGB顺序不一致，这是一个错误。正确的顺序应该是redval、grnval、bluval。
>
> 总体来说，这段代码实现了设置NX背景颜色的功能，但需要修正颜色值的顺序。
>
