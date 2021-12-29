#pragma once

namespace jenkins_one_at_a_time
{
	unsigned int compute(const std::string& data);
	unsigned int compute(const char* key, size_t len);
}
