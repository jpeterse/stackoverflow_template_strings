const char tmplt_title_1[] = "@@first@@ line";
const char tmplt_str_1[] = "Lorem ipsum dolor @@replace@@ consectetur adipiscing elit.";
const char tmplt_title_2[] = "@@second@@ line";
const char tmplt_str_2[] = "Pellentesque consectetur @@me@@ dui sollicitudin nec.";
const char tmplt_title_3[] = "@@third@@ line";
const char tmplt_str_3[] = "Vivamus porttitor @@and me@@ cursus.";

struct tmplt_str_t {
  const char* title;
  const char* msg;
};

const tmplt_str_t tmplt_strs[] = {
  { &tmplt_title_1[0], &tmplt_str_1[0] },
  { &tmplt_title_2[0], &tmplt_str_2[0] },
  { &tmplt_title_3[0], &tmplt_str_3[0] }
};
