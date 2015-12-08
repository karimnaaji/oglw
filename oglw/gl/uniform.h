#include "variant.hpp"
#include "glm/glm.hpp"

namespace OGLW {

using UniformValue = mapbox::util::variant<bool, float, int, glm::vec2,
      glm::vec3, glm::vec4, glm::mat2, glm::mat3, glm::mat4>;

} // OGLW
