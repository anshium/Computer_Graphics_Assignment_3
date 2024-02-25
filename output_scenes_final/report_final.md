# Report File

## Question 1
- Anti-Aliasing Samples = 1: `1804.340942 ms`
- Anti-Aliasing Samples = 32: `55329.074219 ms`

## Question 2
- Scene 1: `16819.386719 ms`
- Scene 2: `16408.328125 ms`
- Scene 3: `16308.259766 ms`
- Scene 4: `26351.888672 ms`

## Question 3
### Method: `uniform_hemisphere_sampling` | SPP: `10` | Anti-Aliasing: `On`
- Small:  `7075.812988 ms`
- Med:  `7760.391113 ms`
- Big:  `9994.512695 ms`
- Many:  `17501.871094 ms`
### Method: `cosine_weighted_sampling` | SPP: `10` | Anti-Aliasing: `On`
- Small:  `5502.695801 ms`
- Med:  `6602.134766 ms`
- Big:  `10504.008789 ms`
- Many:  `14753.739258 ms`
### Method: `light_sampling` | SPP: `10` | Anti-Aliasing: `On`
- Small:  `21898.923828 ms`
- Med:  `22299.101562 ms`
- Big:  `22376.826172 ms`
- Many:  `65068.945312 ms`
### Method: `uniform_hemisphere_sampling` | SPP: `100` | Anti-Aliasing: `On`
- Small:  `48447.414062 ms`
- Med:  `56206.976562 ms`
- Big:  `85078.140625 ms`
- Many:  `210852.468750 ms`
### Method: `cosine_weighted_sampling` | SPP: `100` | Anti-Aliasing: `On`
- Small:  `68122.953125 ms`
- Med:  `86849.640625 ms`
- Big:  `172580.093750 ms`
- Many:  `254728.093750 ms`
### Method: `light_sampling` | SPP: `100` | Anti-Aliasing: `On`
- Small:  `390806.312500 ms`
- Med:  `396981.218750 ms`
- Big:  `394712.156250 ms`
- Many:  `1227124.750000 ms`
### Method: `uniform_hemisphere_sampling` | SPP: `1000` | Anti-Aliasing: `On`
- Small:  `873087.937500 ms`
- Med:  `1015884.312500 ms`
- Big:  `1530626.875000 ms`
- Many:  `2251478.250000 ms`
### Method: `cosine_weighted_sampling` | SPP: `1000` | Anti-Aliasing: `On`
- Small:  `396302.152344 ms`
- Med:  `463583.031250 ms`
- Big:  `550074.292969 ms`
- Many:  `1173603.054688 ms`
### Method: `light_sampling` | SPP: `1000` | Anti-Aliasing: `On`
- Small:  `398668.968750 ms`
- Med:  `406222.250000 ms`
- Big:  `410246.843750 ms`
- Many:  `1320913.250000 ms`


## Report Questions

### Question 1
**Why can’t we render point and directional lights with uniform hemisphere sampling or cosine weighted sampling?**

For both point and directional lights, the set of directions that would contribute to the final color and intensity of a pixel are definite and known.

If we use uniform hemisphere sampling or cosine weighted sampling, we would be considering many directions that would not contribute to the the properties of the pixel being currently considered.

This would take a much higher time (almost infinity) to converge than simply considering all the valid directions that the light can take. For point lights this would be the direction to the point light from the point of intersection and for directional lights, this would be the constant fixed direction.

### Question 2
**Why does the noise increase for the same number of samples in the case of uniform hemisphere and cosine weighted sampling as the size of the area light decreases?**

The noise increases for the same number of samples in the case of uniform hemisphere and cosine sampling as a smaller number of rays hit the light when when the light's size is small. Consider a ray x. The ray had an associated direction `d`. In essense the number of directions that would intersect the light surface in the case of area light would be less.

Hence, the total number of samples that hit a light source directly or after intersecting a surface are fewer when the light dimensions are less. Hence we would have to sample more number of light rays. This means that for the same spp, we would have more noise for images having a light source with a smaller size.