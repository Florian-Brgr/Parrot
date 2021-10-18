Parrot is a GraphicsEngine for Windows.

General structure:
  - create at least one Window
  - attach a Scene to each Window
  - when a Scene is attached to a Window, it gets updated each frame and also rendered to the Window
  
  - each Scene holds several SceneObjects
  - each SceneObject holds several Components (Transform-Component is mandatory)
  - SceneObjects can hold a scriptable Component that make them highly customizable