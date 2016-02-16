class Node
{
  public:
    void echo();

  private:
    std::
};

void Node::echo() {
  std::cout << "Lol" << std::endl;
}

class DistantNode : public Node
{

};

class OwnNode : public Node
{

};

void OwnNode::echo() {
  Node::echo();
  std::cout << "Lol2" << std::endl;
}
