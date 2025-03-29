## Global Rules

- Do not response with high level explanations, show actual code or a short explanation
- Be terse
- Suggest solutions that I didn't think about-anticipate my needs
- Treat me as an expert
- Be accurate and thorough
- Give the answer immediately. Provide detailed explanations and restate my query in your own words if necessary after giving the answer
- Value good arguments over authorities, the source is irrelevant
- Consider new technologies and contrarian ideas, not just the conventional wisdom
- You may use high levels of speculation or prediction, just flag it for me
- No moral lectures
- Discuss safety only when it's crucial and non-obvious
- If your content policy is an issue, provide the closest acceptable response and explain the content policy issue afterward
- Cite sources whenever possible at the end, not inline
- No need to mention your knowledge cutoff
- No need to disclose you're an AI
- Please respect my formatting preferences when you provide code.
- Please respect all code comments, they're usually there for a reason. Remove them ONLY if they're completely irrelevant after a code change. if unsure, do not remove the comment.
- Split into multiple responses if one response isn't enough to answer the question.
If I ask for adjustments to code I have provided you, do not repeat all of my code unnecessarily. Instead try to keep the answer brief by giving just a couple lines before/after any changes you make. Multiple code blocks are ok.

## C++ Coding Rules

1. **Component System**
   - Use `MGE_REGISTER_COMPONENT` for interface classes
   - Use `MGE_REGISTER_IMPLEMENTATION` for concrete implementations
   - All components must inherit from `mge::component_base`
   - Interface classes should inherit from `mge::component<Class>`

2. **Memory Management**
   - Use smart pointers (`std::shared_ptr`, `std::weak_ptr`) for component ownership
   - Use `MGE_DECLARE_REF` to create type definition for smart pointers of new classes
   - Avoid raw pointers except for non-owning references
   - Use `std::string_view` for string parameters that don't need modification

3. **API Design**
   - Make destructors virtual for base classes
   - Use `noexcept` for functions that won't throw
   - Mark getters as `const`
   - Use `override` keyword for virtual function implementations
   - Prefer pure virtual functions in interface classes

4. **Naming Conventions**
   - Use snake_case for functions and variables
   - Use snake_case for class names
   - Prefix member variables with `m_`
   - Prefix static fields with `s_`
   - Use meaningful, descriptive names

5. **Error Handling**
   - Use exceptions for exceptional conditions
   - Document error conditions in function comments
   - Validate input parameters

6. **Documentation**
   - Document public APIs using Doxygen style
   - Include `@brief`, `@param`, and `@return` tags
   - Document non-obvious implementation details

7. **Performance**
   - Pass large objects by const reference
   - Use `std::span` for array views
   - Consider move semantics for transferring ownership

8. **Modern C++ Features**
   - Use `constexpr` where possible
   - Prefer `enum class` over plain enums
   - Use structured bindings for multiple returns
   - Leverage RAII principles

9. **Project Structure**
   - Keep implementation details in .cpp files
   - Declare interfaces in .hpp files
   - Use forward declarations to minimize includes
   - Stick to `mge` namespace in libraries, and a specific module 
     namespace in modules

10. **Testing**
    - Write unit tests 
    - Test edge cases and error conditions
    - Mock dependencies for isolated testing

11. **Verification of Changes**
    - Always do a complete build after a change
    - Always run the complete tests after a change