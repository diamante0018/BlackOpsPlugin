#pragma once

namespace command {

class params {
public:
  params();

  [[nodiscard]] int size() const;
  [[nodiscard]] const char* get(int index) const;
  [[nodiscard]] std::string join(int index) const;

  const char* operator[](const int index) const { return this->get(index); }

private:
  int nesting_;
};

class params_sv {
public:
  params_sv();

  [[nodiscard]] int size() const;
  [[nodiscard]] const char* get(int index) const;
  [[nodiscard]] std::string join(int index) const;

  const char* operator[](const int index) const { return this->get(index); }

private:
  int nesting_;
};

using command_param_function = std::function<void(const params&)>;
using sv_command_param_function = std::function<void(const params_sv&)>;

void add_raw(const char* name, void (*callback)());
void add_sv(const char* name, const sv_command_param_function& callback);

void execute(std::string command, bool sync = false);
} // namespace command
