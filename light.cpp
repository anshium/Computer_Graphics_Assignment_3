#include "light.h"

Light::Light(LightType type, nlohmann::json config) {
    switch (type) {
        case LightType::POINT_LIGHT:
            this->position = Vector3f(config["location"][0], config["location"][1], config["location"][2]);
            break;
        case LightType::DIRECTIONAL_LIGHT:
            this->direction = Vector3f(config["direction"][0], config["direction"][1], config["direction"][2]);
            break;
        case LightType::AREA_LIGHT:
            // DONE: Implement this
            this->center = Vector3f(config["center"][0], config["center"][1], config["center"][2]);
            this->vx = Vector3f(config["vx"][0], config["vx"][1], config["vx"][2]);
            this->vy = Vector3f(config["vy"][0], config["vy"][1], config["vy"][2]);
            this->normal = Vector3f(config["normal"][0], config["normal"][1], config["normal"][2]);
            break;
        default:
            std::cout << "WARNING: Invalid light type detected";
            break;
    }

    this->radiance = Vector3f(config["radiance"][0], config["radiance"][1], config["radiance"][2]);
    this->type = type;
}

std::pair<Vector3f, LightSample> Light::sample(Interaction *si) {
    LightSample ls;
    memset(&ls, 0, sizeof(ls));

    Vector3f radiance;
    switch (type) {
        case LightType::POINT_LIGHT:
            ls.wo = (position - si->p);
            ls.d = ls.wo.Length();
            ls.wo = Normalize(ls.wo);
            radiance = (1.f / (ls.d * ls.d)) * this->radiance;
            break;
        case LightType::DIRECTIONAL_LIGHT:
            ls.wo = Normalize(direction);
            ls.d = 1e10;
            radiance = this->radiance;
            break;
        case LightType::AREA_LIGHT:
            // TODO: Implement this
            
            radiance = this->radiance;
            Vector2f u = Vector2f(next_float(), next_float());
            
            float theta = M_PI / 2 * next_float();
            float phi   = 2 * M_PI * next_float();

            ls.wo = Normalize(Vector3f(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta)));

            ls.d = ls.wo.Length();

            break;
    }
    return { radiance, ls };
}

Interaction Light::intersectLight(Ray *ray) {
    Interaction si;
    memset(&si, 0, sizeof(si));

    if (type == LightType::AREA_LIGHT) {
        // DONE: Implement this

        // First check ray plane intersection
        float dDotN = Dot(ray->d, this->normal);
        if (dDotN != 0.f) {
            float t = -Dot((ray->o - this->center), this->normal) / dDotN;

            if (t >= 0.f) {
                si.didIntersect = true;
                si.t = t;
                si.n = this->normal;
                si.p = ray->o + ray->d * si.t;
            }
        }

        // Then check the bounds
        if(si.didIntersect){
            Vector3f vx_to_p = si.p - this->center;
            Vector3f vy_to_p = si.p - this->center;

            float vx_proj = std::abs(Dot(vx_to_p, this->vx) / (this->vx).LengthSquared());
            float vy_proj = std::abs(Dot(vy_to_p, this->vy) / (this->vy).LengthSquared()); 

            if (vx_proj >= 0.f && vx_proj <= 1.f && vy_proj >= 0.f && vy_proj <= 1.f) {
                si.didIntersect = true;
                si.emissiveColor = this->radiance;
            } else {
                si.didIntersect = false;
            }
        }

        // si.emissiveColor = Vector3f(1, 1, 1);
    }

    return si;
}