#include "render.h"

Integrator::Integrator(Scene &scene)
{
    this->scene = scene;
    this->outputImage.allocate(TextureType::UNSIGNED_INTEGER_ALPHA, this->scene.imageResolution);
}

long long Integrator::render()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int x = 0; x < this->scene.imageResolution.x; x++) {
        for (int y = 0; y < this->scene.imageResolution.y; y++) {
            /** For the light intersection part **/
            Vector3f result = Vector3f(0, 0, 0);
            double PDF = 1.0;
            bool isLightPoint = false;
            Vector3f lightemissivecolor;

            Vector3f pixelPoint = this->scene.camera.upperLeft + (0.5 * this->scene.camera.pixelDeltaU + 0.5 * this->scene.camera.pixelDeltaV);
            pixelPoint = pixelPoint + x * this->scene.camera.pixelDeltaU + y * this->scene.camera.pixelDeltaV;

            Vector3f direction = Normalize(pixelPoint - this->scene.camera.from);

            Ray a = Ray(this->scene.camera.from, direction);
            Interaction lsi = this->scene.rayEmitterIntersect(a);
            if(lsi.didIntersect){
                isLightPoint = true;
                result += lsi.emissiveColor;
                this->outputImage.writePixelColor(result, x, y);
                continue;

                // Eech, with all this drama, the light spilling got fixed.
            }
            /* Light intersection part ends*/

            for(int anti_aliasing_sample = 0; anti_aliasing_sample < NUM_ANTI_ALIASING_SAMPLES; anti_aliasing_sample++){
                // Phele ham ye dekh rahen hain ki camera wali ray kahin intersect hui?
                // for(int i = 0; i < 10; i++){
                Ray cameraRay = this->scene.camera.generateRay(x, y);
                Interaction si = this->scene.rayIntersect(cameraRay);
                // Agar intersect hui to ham dekhenge ki kahan intersect hui.


                if(si.didIntersect){
                    // wahan se ham light ko sample karenge aur ek ray banayenge
                    Vector3f radiance;
                    LightSample ls;
                    
                    for(Light &light : this->scene.lights){
                        if(light.type == AREA_LIGHT){
                            for(int sampling_iteration = 0; sampling_iteration < spp; sampling_iteration++){
                                std::tie(radiance, ls) = light.sample(&si);
                                // aur dekhenge ki kisi light se intersect kiya ki nahi

                                // Iske liye, phele ham shadow ray banayenge
                                Ray lightRay(si.p + 1e-3 * si.n, ls.wo);
                                Interaction siLR = this->scene.rayEmitterIntersect(lightRay);

                                if(siLR.didIntersect){
                                    Ray shadowRay(si.p + 1e-3 * si.n, ls.wo);
                                    Interaction siSR = this->scene.rayIntersect(shadowRay);
                                    if(!siSR.didIntersect || (siSR.p - si.p).Length() > (siLR.p - si.p).Length()){
                                        if(sampling_method == UniformHemisphereSampling){
                                            result += si.bsdf->eval(&si, si.toLocal(ls.wo)) * siLR.emissiveColor * std::abs(Dot(si.n, ls.wo));
                                        }
                                        else if(sampling_method == CosineWeightedSampling){
                                            result += si.bsdf->eval(&si, si.toLocal(ls.wo)) * siLR.emissiveColor * M_PI;
                                        } 
                                        else if(sampling_method == LightSampling){
                                            result += si.bsdf->eval(&si, si.toLocal(ls.wo)) * siLR.emissiveColor * std::abs(Dot(si.n, ls.wo)) * std::abs(Dot(Normalize(light.normal), -ls.wo)) / (ls.d * ls.d);
                                        }
                                    }
                                }
                                PDF = light.PDF(sampling_method);
                            }
                        }
                        else{
                            std::tie(radiance, ls) = light.sample(&si);
                        
                            Ray shadowRay(si.p + 1e-3f * si.n, ls.wo);
                            Interaction siShadow = this->scene.rayIntersect(shadowRay);

                            if (!siShadow.didIntersect || siShadow.t > ls.d) {
                                result += si.bsdf->eval(&si, si.toLocal(ls.wo)) * radiance * std::abs(Dot(si.n, ls.wo)) + si.emissiveColor;
                            }
                            if(anti_aliasing_sample == NUM_ANTI_ALIASING_SAMPLES - 1){
                                result = result * spp;
                            }
                        }
                    }
                }
                // Agar light source se kiya, to sahi hai, tab ham shade kardenge equation (2) ke hisaab se.

                // ye karke dekhta hun, isme shadow ray ki kahani nahi hai, I mean hai par waisi wali shadow ray nahi shayad
            }

            // Could have built this into the formula, but thought to do the following
            // if(isLightPoint){
            //     this->outputImage.writePixelColor(result, x, y);
            // }
            // else{
            //     this->outputImage.writePixelColor(result * (1 / PDF) / spp / NUM_ANTI_ALIASING_SAMPLES, x, y);
            // }
            // this->outputImage.writePixelColor(result * (1 / PDF) / spp / NUM_ANTI_ALIASING_SAMPLES, x, y);
            this->outputImage.writePixelColor(result * (1 / PDF) / spp / NUM_ANTI_ALIASING_SAMPLES, x, y);
            // Vector3f result(0, 0, 0);
            // for(int sampling_iteration = 0; sampling_iteration < spp; sampling_iteration++){
            //     Ray cameraRay = this->scene.camera.generateRay(x, y);
            //     Interaction si = this->scene.rayIntersect(cameraRay);

            //     if (si.didIntersect) {
            //         Vector3f radiance;
            //         LightSample ls;
            //         for (Light &light : this->scene.lights) {
            //             std::tie(radiance, ls) = light.sample(&si);
                        
            //             Ray shadowRay(si.p + 1e-3f * si.n, ls.wo);
            //             Interaction siShadow = this->scene.rayIntersect(shadowRay);

            //             if (!siShadow.didIntersect || siShadow.t > ls.d) {
            //                 if(this->scene.rayEmitterIntersect(shadowRay).didIntersect){
            //                     result += si.bsdf->eval(&si, ls.wo) * radiance * std::abs(Dot(si.n, ls.wo)) + siShadow.emissiveColor;
            //                 }
            //             }
            //         }
            //     }
            // }
            // this->outputImage.writePixelColor(result / spp, x, y);
        }
    }
    auto finishTime = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();
}

int spp;
int sampling_method;

int main(int argc, char **argv)
{
    if (argc != 5) {
        std::cerr << "Usage: ./render <scene_config> <out_path> <num_samples> <sampling_strategy>";
        return 1;
    }
    Scene scene(argv[1]);

    Integrator rayTracer(scene);
    spp = atoi(argv[3]);
    sampling_method = atoi(argv[4]);
    auto renderTime = rayTracer.render();
    
    std::cout << "Render Time: " << std::to_string(renderTime / 1000.f) << " ms" << std::endl;
    rayTracer.outputImage.save(argv[2]);

    return 0;
}
