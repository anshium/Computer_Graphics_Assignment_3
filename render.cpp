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

            Vector3f result = Vector3f(0, 0, 0);
            // Phele ham ye dekh rahen hain ki camera wali ray kahin intersect hui?
            Ray cameraRay = this->scene.camera.generateRay(x, y);
            Interaction si = this->scene.rayIntersect(cameraRay);
            // Agar intersect hui to ham dekhenge ki kahan intersect hui.

            Interaction lsi = this->scene.rayEmitterIntersect(cameraRay);

            if(si.didIntersect){
                // wahan se ham light ko sample karenge aur ek ray banayenge
                Vector3f radiance;
                LightSample ls;

                for(Light &light : this->scene.lights){
                    for(int sampling_iteration = 0; sampling_iteration < spp; sampling_iteration++){
                        std::tie(radiance, ls) = light.sample(&si);
                        // aur dekhenge ki kisi light se intersect kiya ki nahi

                        // Iske liye, phele ham shadow ray banayenge
                        Ray lightRay(si.p + 1e-3 * si.n, ls.wo);
                        Interaction siLR = light.intersectLight(&lightRay);

                        if(siLR.didIntersect){
                            Ray shadowRay(si.p + 1e-3 * si.n, ls.wo);
                            Interaction siSR = this->scene.rayIntersect(shadowRay);
                            if(!siSR.didIntersect || (siSR.p - si.p).Length() > (siLR.p - si.p).Length()){
                                result += si.bsdf->eval(&si, si.toLocal(ls.wo)) * siLR.emissiveColor * std::abs(Dot(si.n, ls.wo)); 
                            }
                        }
                    }
                }

                // Agar light source se kiya, to sahi hai, tab ham shade kardenge equation (2) ke hisaab se.

                // ye karke dekhta hun, isme shadow ray ki kahani nahi hai, I mean hai par waisi wali shadow ray nahi shayad
                }
                if(lsi.didIntersect){
                    result += lsi.emissiveColor;
                }


                this->outputImage.writePixelColor(result * (2 * M_PI) / spp, x, y);
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
