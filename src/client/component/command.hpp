#pragma once

namespace command {

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

void add_raw(const char* name, void (*callback)());
void add(const char* name,
         const std::function<void(const params_sv&)>& callback);

void execute(std::string command, bool sync = false);
} // namespace command
