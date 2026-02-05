# Design Patterns Table Implementation

This project implements a dynamic `Table` data structure in C++20, demonstrating Design Patterns such as Factory and Singleton (via concepts). It uses type erasure `IValue` to handle heterogeneous data types safe and effectively.

## Github Actions' Notes for Future Reference

### Concept
**Continuous Integration (CI) and Continuous Deployment (CD)** are practices that automate the integration of code changes and the delivery of software.
*   **CI** ensures that your code is built and tested automatically whenever you push changes, catching errors early.
*   **CD** takes this a step further by automatically deploying your application or, in this case, your documentation.

In this project, we use GitHub Actions to implement a documentation pipeline. Whenever you push code, a runner (a virtual server) automatically:
1.  Checks out your code.
2.  Installs documentation tools (`doxygen`, `graphviz`).
3.  Generates the documentation website.
4.  Deploys it to a publicly accessible branch.

### Workflow
The workflow is configured in `.github/workflows/documentation.yml`. It employs a **branch-based deployment strategy**:
*   Documentation is deployed to the `gh-pages` branch.
*   Each branch in your repository gets its own folder on the documentation site.
*   **Example**:
    *   `main` branch docs -> `https://BaraWazwaz.github.io/Design-Patterns/main/`
    *   `dev` branch docs -> `https://BaraWazwaz.github.io/Design-Patterns/dev/`

This allows you to view documentation for different versions or features in parallel without one overwriting the other.

### Setup
To enable this automation for your repository:

1.  **Configure Action Permissions**:
    *   Go to **Settings** > **Actions** > **General**.
    *   Under **Action permissions**, select **Allow all actions and reusable workflows**.
    *   *Why?* The default setting might be too restrictive, blocking standard actions like `actions/checkout`.

2.  **Push your changes** to GitHub:
    ```bash
    git add .
    git commit -m "Add documentation workflow"
    git push
    ```
3.  Wait for the **Action** to run successfully (check the "Actions" tab in your repository). This will create the `gh-pages` branch.
4.  Go to your GitHub repository **Settings** > **Pages**.
5.  Under **Build and deployment**:
    *   Set **Source** to `Deploy from a branch`.
    *   Set **Branch** to `gh-pages` and folder to `/ (root)`.
6.  Click **Save**.
7.  